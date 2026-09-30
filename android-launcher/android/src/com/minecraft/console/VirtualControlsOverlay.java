package com.minecraft.console;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.RectF;
import android.os.SystemClock;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import org.libsdl.app.SDLActivity;
import org.libsdl.app.SDLSurface;

import java.io.File;
import java.io.FileInputStream;

public class VirtualControlsOverlay extends View {

    private final SDLSurface mSurface;
    private final Paint mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Paint mBorderPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Paint mTextPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Paint mFpsPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Path mPath = new Path();

    // Settings (loaded from options.txt)
    // touchControlStyle: 0: Modern Bedrock, 1: Classic PE D-Pad, 2: Joystick + Action
    private int mControlStyle = 0;
    // touchControlScale: 0: 80%, 1: 100%, 2: 125%, 3: 150%
    private int mControlScale = 1;
    // touchControlOpacity: 0: 35%, 1: 65%, 2: 90%, 3: 100%
    private int mControlOpacity = 1;

    private long mLastOptionsCheck = 0;
    private long mLastOptionsMtime = 0;

    // Button states
    public static class VButton {
        String label;
        int keyCode;
        RectF bounds = new RectF();
        boolean pressed = false;
        int pointerId = -1;
        boolean isCircle = false;
        int iconType = 0; // 0: text, 1: arrow-up, 2: arrow-down, 3: arrow-left, 4: arrow-right, 5: sneak, 6: sword, 7: hand

        VButton(String label, int keyCode) {
            this.label = label;
            this.keyCode = keyCode;
        }

        VButton(String label, int keyCode, int iconType) {
            this.label = label;
            this.keyCode = keyCode;
            this.iconType = iconType;
        }
    }

    // Directional buttons
    private final VButton btnUp = new VButton("▲", KeyEvent.KEYCODE_W, 1);
    private final VButton btnDown = new VButton("▼", KeyEvent.KEYCODE_S, 2);
    private final VButton btnLeft = new VButton("◀", KeyEvent.KEYCODE_A, 3);
    private final VButton btnRight = new VButton("▶", KeyEvent.KEYCODE_D, 4);
    private final VButton btnCenter = new VButton("◆", KeyEvent.KEYCODE_SHIFT_LEFT, 5); // Sneak

    // Action buttons
    private final VButton btnJump = new VButton("▲", KeyEvent.KEYCODE_SPACE, 1);
    private final VButton btnMine = new VButton("⚔", 0, 6); // Attack / Break
    private final VButton btnUse = new VButton("👆", 0, 7);       // Place / Interact
    private final VButton btnInv = new VButton("INV", KeyEvent.KEYCODE_E, 0);     // Inventory
    private final VButton btnDrop = new VButton("DROP", KeyEvent.KEYCODE_Q, 0);   // Drop item

    // Menu / Utility buttons
    private final VButton btnPause = new VButton("⏸", KeyEvent.KEYCODE_ESCAPE, 0);  // Pause
    private final VButton btnF5 = new VButton("F5", KeyEvent.KEYCODE_F5, 0);        // Perspective
    private final VButton btnToggleUI = new VButton("🎮", 0, 0);                   // Toggle overlay visibility

    private final VButton[] allButtons = new VButton[] {
        btnUp, btnDown, btnLeft, btnRight, btnCenter,
        btnJump, btnMine, btnUse, btnInv, btnDrop,
        btnPause, btnF5, btnToggleUI
    };

    // Joystick state for Mode 2
    private float mJoyCenterX = 0;
    private float mJoyCenterY = 0;
    private float mJoyKnobX = 0;
    private float mJoyKnobY = 0;
    private float mJoyBaseRadius = 0;
    private float mJoyKnobRadius = 0;
    private int mJoyPointerId = -1;
    private boolean mJoyActive = false;
    private boolean mJoyUpPressed = false;
    private boolean mJoyDownPressed = false;
    private boolean mJoyLeftPressed = false;
    private boolean mJoyRightPressed = false;

    private boolean mControlsVisible = false;
    private boolean mInGameRelativeMouse = false;
    private boolean mManualForceVisible = false;
    private float mDensity = 1.0f;

    // FPS Counter variables
    private int mFrameCount = 0;
    private long mLastFpsTime = 0;
    private int mCurrentFps = 60;
    private final RectF mFpsBox = new RectF();

    public VirtualControlsOverlay(Context context, SDLSurface surface) {
        super(context);
        this.mSurface = surface;
        this.mDensity = context.getResources().getDisplayMetrics().density;

        mPaint.setStyle(Paint.Style.FILL);

        mBorderPaint.setStyle(Paint.Style.STROKE);
        mBorderPaint.setStrokeWidth(2.5f * mDensity);

        mTextPaint.setColor(Color.WHITE);
        mTextPaint.setTextAlign(Paint.Align.CENTER);
        mTextPaint.setFakeBoldText(true);

        mFpsPaint.setColor(Color.GREEN);
        mFpsPaint.setTextAlign(Paint.Align.LEFT);
        mFpsPaint.setFakeBoldText(true);
        mFpsPaint.setTextSize(14 * mDensity);

        setFocusable(false);
        setFocusableInTouchMode(false);

        SDLActivity.mRelativeMouseCallback = new SDLActivity.RelativeMouseCallback() {
            @Override
            public void onRelativeMouseChanged(final boolean enabled) {
                post(new Runnable() {
                    @Override
                    public void run() {
                        mInGameRelativeMouse = enabled;
                        mControlsVisible = mManualForceVisible || mInGameRelativeMouse;
                        invalidate();
                    }
                });
            }
        };

        checkAndReloadOptions();
    }

    private float getScaleFactor() {
        switch (mControlScale) {
            case 0: return 0.80f;
            case 2: return 1.25f;
            case 3: return 1.50f;
            default: return 1.00f;
        }
    }

    private int getControlAlpha() {
        switch (mControlOpacity) {
            case 0: return 90;   // 35%
            case 2: return 230;  // 90%
            case 3: return 255;  // 100%
            default: return 165; // 65%
        }
    }

    private void checkAndReloadOptions() {
        long now = SystemClock.uptimeMillis();
        if (now - mLastOptionsCheck < 500) return;
        mLastOptionsCheck = now;

        File optFile = new File("/sdcard/LegacyMCPE/.4jcraft/options.txt");
        if (!optFile.exists()) {
            optFile = new File("/sdcard/LegacyMCPE/options.txt");
        }
        if (!optFile.exists()) return;

        long mtime = optFile.lastModified();
        if (mtime == mLastOptionsMtime) return;
        mLastOptionsMtime = mtime;

        try {
            int len = (int) optFile.length();
            if (len <= 0 || len > 200000) return;
            byte[] data = new byte[len];
            FileInputStream fis = new FileInputStream(optFile);
            fis.read(data);
            fis.close();

            String content;
            if (len >= 2 && ((data[0] == 0 && data[1] != 0) || (data[0] != 0 && data[1] == 0))) {
                content = new String(data, "UTF-16BE");
                if (!content.contains("touchControlStyle") && !content.contains("music:")) {
                    content = new String(data, "UTF-16LE");
                }
            } else {
                content = new String(data, "UTF-8");
            }

            int style = mControlStyle;
            int scale = mControlScale;
            int opacity = mControlOpacity;

            for (String line : content.split("\n")) {
                line = line.trim();
                if (line.startsWith("touchControlStyle:")) {
                    try { style = Integer.parseInt(line.substring("touchControlStyle:".length()).trim()); } catch (Exception ignored) {}
                } else if (line.startsWith("touchControlScale:")) {
                    try { scale = Integer.parseInt(line.substring("touchControlScale:".length()).trim()); } catch (Exception ignored) {}
                } else if (line.startsWith("touchControlOpacity:")) {
                    try { opacity = Integer.parseInt(line.substring("touchControlOpacity:".length()).trim()); } catch (Exception ignored) {}
                }
            }

            if (style != mControlStyle || scale != mControlScale || opacity != mControlOpacity) {
                mControlStyle = style;
                mControlScale = scale;
                mControlOpacity = opacity;
                post(new Runnable() {
                    @Override
                    public void run() {
                        layoutButtons(getWidth(), getHeight());
                        invalidate();
                    }
                });
            }
        } catch (Throwable ignored) {
        }
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        layoutButtons(w, h);
    }

    private void layoutButtons(int w, int h) {
        if (w <= 0 || h <= 0) return;
        float d = mDensity;
        float scale = getScaleFactor();

        // Top bar buttons (Pause, F5, Toggle UI, FPS box)
        float topY = 12 * d;
        float topBtnW = 38 * d;
        float topBtnH = 30 * d;

        float pauseX = w * 0.5f - topBtnW * 0.5f;
        btnPause.bounds.set(pauseX, topY, pauseX + topBtnW, topY + topBtnH);
        btnPause.isCircle = false;

        float f5X = pauseX - topBtnW - 12 * d;
        btnF5.bounds.set(f5X, topY, f5X + topBtnW, topY + topBtnH);
        btnF5.isCircle = false;

        float togX = w - 48 * d;
        btnToggleUI.bounds.set(togX, topY, togX + topBtnW, topY + topBtnH);
        btnToggleUI.isCircle = false;

        mFpsBox.set(14 * d, topY, 14 * d + 84 * d, topY + topBtnH);

        if (mControlStyle == 0 || mControlStyle == 1) {
            // Style 0 (Modern Bedrock - Separated) & Style 1 (Classic PE - Connected Cross)
            float padSize = (mControlStyle == 0 ? 56 : 52) * d * scale;
            float padLeft = 24 * d;
            float padBottom = h - 24 * d;
            float dpadCenterX = padLeft + padSize * 1.5f;
            float dpadCenterY = padBottom - padSize * 1.5f;

            float gap = (mControlStyle == 0) ? (4 * d * scale) : 0;

            btnCenter.bounds.set(dpadCenterX - padSize * 0.5f, dpadCenterY - padSize * 0.5f,
                                 dpadCenterX + padSize * 0.5f, dpadCenterY + padSize * 0.5f);
            btnCenter.isCircle = (mControlStyle == 1);

            btnUp.bounds.set(dpadCenterX - padSize * 0.5f, dpadCenterY - padSize * 1.5f - gap,
                             dpadCenterX + padSize * 0.5f, dpadCenterY - padSize * 0.5f - gap);
            btnDown.bounds.set(dpadCenterX - padSize * 0.5f, dpadCenterY + padSize * 0.5f + gap,
                               dpadCenterX + padSize * 0.5f, dpadCenterY + padSize * 1.5f + gap);
            btnLeft.bounds.set(dpadCenterX - padSize * 1.5f - gap, dpadCenterY - padSize * 0.5f,
                              dpadCenterX - padSize * 0.5f - gap, dpadCenterY + padSize * 0.5f);
            btnRight.bounds.set(dpadCenterX + padSize * 0.5f + gap, dpadCenterY - padSize * 0.5f,
                               dpadCenterX + padSize * 1.5f + gap, dpadCenterY + padSize * 0.5f);

            btnUp.isCircle = false;
            btnDown.isCircle = false;
            btnLeft.isCircle = false;
            btnRight.isCircle = false;

            // Action buttons on bottom-right
            float rightMargin = w - 24 * d;
            float btnSize = 58 * d * scale;

            // Jump button (big, bottom right)
            float jumpX = rightMargin - btnSize * 1.4f;
            float jumpY = h - 28 * d - btnSize * 1.4f;
            btnJump.bounds.set(jumpX, jumpY, jumpX + btnSize * 1.35f, jumpY + btnSize * 1.35f);
            btnJump.isCircle = (mControlStyle == 1);

            // Mine button (attack)
            float mineX = jumpX - btnSize * 1.05f;
            float mineY = jumpY - btnSize * 0.5f;
            btnMine.bounds.set(mineX, mineY, mineX + btnSize * 0.95f, mineY + btnSize * 0.95f);
            btnMine.isCircle = false;

            // Use button (interact / place)
            float useX = jumpX;
            float useY = jumpY - btnSize * 1.05f;
            btnUse.bounds.set(useX, useY, useX + btnSize * 0.95f, useY + btnSize * 0.95f);
            btnUse.isCircle = false;

            // Inventory button
            float invX = mineX - btnSize * 0.8f;
            float invY = jumpY + btnSize * 0.25f;
            btnInv.bounds.set(invX, invY, invX + btnSize * 0.75f, invY + btnSize * 0.75f);
            btnInv.isCircle = false;

            // Drop button
            float dropX = mineX;
            float dropY = mineY - btnSize * 0.85f;
            btnDrop.bounds.set(dropX, dropY, dropX + btnSize * 0.75f, dropY + btnSize * 0.75f);
            btnDrop.isCircle = false;

        } else if (mControlStyle == 2) {
            // Style 2: Joystick + Action Buttons (Image 3)
            mJoyBaseRadius = 64 * d * scale;
            mJoyKnobRadius = 26 * d * scale;
            mJoyCenterX = 28 * d + mJoyBaseRadius;
            mJoyCenterY = h - 28 * d - mJoyBaseRadius;
            if (!mJoyActive) {
                mJoyKnobX = mJoyCenterX;
                mJoyKnobY = mJoyCenterY;
            }

            // Hide directional D-Pad bounds so they don't capture touch directly
            btnUp.bounds.set(0, 0, 0, 0);
            btnDown.bounds.set(0, 0, 0, 0);
            btnLeft.bounds.set(0, 0, 0, 0);
            btnRight.bounds.set(0, 0, 0, 0);

            // Right action cluster: Sword, Hand, Jump, Sneak
            float rightMargin = w - 24 * d;
            float btnSize = 52 * d * scale;

            // Jump button (top right of cluster)
            float jumpX = rightMargin - btnSize;
            float jumpY = h - 30 * d - btnSize * 2.1f;
            btnJump.bounds.set(jumpX, jumpY, jumpX + btnSize, jumpY + btnSize);
            btnJump.isCircle = false;

            // Sneak / Descend button (below Jump)
            float sneakX = jumpX;
            float sneakY = jumpY + btnSize * 1.15f;
            btnCenter.bounds.set(sneakX, sneakY, sneakX + btnSize, sneakY + btnSize);
            btnCenter.isCircle = false;
            btnCenter.iconType = 2; // Down arrow

            // Mine (Sword) button (left of Jump)
            float swordX = jumpX - btnSize * 1.15f;
            float swordY = jumpY;
            btnMine.bounds.set(swordX, swordY, swordX + btnSize, swordY + btnSize);
            btnMine.isCircle = false;

            // Use (Hand) button (below Sword)
            float handX = swordX;
            float handY = sneakY;
            btnUse.bounds.set(handX, handY, handX + btnSize, handY + btnSize);
            btnUse.isCircle = false;

            // Inventory & Drop
            float invX = swordX - btnSize * 1.0f;
            float invY = handY;
            btnInv.bounds.set(invX, invY, invX + btnSize * 0.8f, invY + btnSize * 0.8f);
            btnInv.isCircle = false;

            float dropX = invX;
            float dropY = swordY;
            btnDrop.bounds.set(dropX, dropY, dropX + btnSize * 0.8f, dropY + btnSize * 0.8f);
            btnDrop.isCircle = false;
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        checkAndReloadOptions();

        // Calculate real FPS
        long now = SystemClock.uptimeMillis();
        mFrameCount++;
        if (now - mLastFpsTime >= 500) {
            mCurrentFps = (int) (mFrameCount * 1000.0f / (now - mLastFpsTime));
            mFrameCount = 0;
            mLastFpsTime = now;
        }

        // Draw FPS Counter Box
        mPaint.setColor(Color.argb(160, 20, 20, 20));
        canvas.drawRoundRect(mFpsBox, 6 * mDensity, 6 * mDensity, mPaint);

        mFpsPaint.setColor(mCurrentFps >= 45 ? Color.GREEN : (mCurrentFps >= 25 ? Color.YELLOW : Color.RED));
        canvas.drawText("FPS: " + mCurrentFps, mFpsBox.left + 8 * mDensity, mFpsBox.centerY() + 5 * mDensity, mFpsPaint);

        // Draw Toggle Button always
        drawMinecraftButton(canvas, btnToggleUI, 180);

        // Draw virtual controls if enabled
        if (mControlsVisible) {
            int alpha = getControlAlpha();

            if (mControlStyle == 0) {
                // Style 0: Modern Bedrock (Separated buttons)
                drawMinecraftButton(canvas, btnUp, alpha);
                drawMinecraftButton(canvas, btnDown, alpha);
                drawMinecraftButton(canvas, btnLeft, alpha);
                drawMinecraftButton(canvas, btnRight, alpha);
                drawMinecraftButton(canvas, btnCenter, alpha);

                drawMinecraftButton(canvas, btnJump, alpha);
                drawMinecraftButton(canvas, btnMine, alpha);
                drawMinecraftButton(canvas, btnUse, alpha);
                drawMinecraftButton(canvas, btnInv, alpha);
                drawMinecraftButton(canvas, btnDrop, alpha);
                drawMinecraftButton(canvas, btnPause, alpha);
                drawMinecraftButton(canvas, btnF5, alpha);

            } else if (mControlStyle == 1) {
                // Style 1: Classic MCPE (Connected cross D-pad)
                drawClassicCrossDpad(canvas, alpha);

                drawMinecraftButton(canvas, btnJump, alpha);
                drawMinecraftButton(canvas, btnMine, alpha);
                drawMinecraftButton(canvas, btnUse, alpha);
                drawMinecraftButton(canvas, btnInv, alpha);
                drawMinecraftButton(canvas, btnDrop, alpha);
                drawMinecraftButton(canvas, btnPause, alpha);
                drawMinecraftButton(canvas, btnF5, alpha);

            } else if (mControlStyle == 2) {
                // Style 2: Joystick + Action buttons
                drawJoystick(canvas, alpha);

                drawMinecraftButton(canvas, btnMine, alpha);
                drawMinecraftButton(canvas, btnUse, alpha);
                drawMinecraftButton(canvas, btnJump, alpha);
                drawMinecraftButton(canvas, btnCenter, alpha);
                drawMinecraftButton(canvas, btnInv, alpha);
                drawMinecraftButton(canvas, btnDrop, alpha);
                drawMinecraftButton(canvas, btnPause, alpha);
                drawMinecraftButton(canvas, btnF5, alpha);
            }
        }

        // Request next frame to keep FPS counter updated
        postInvalidateDelayed(16);
    }

    // Authentic Minecraft 3D Beveled Stone Button
    private void drawMinecraftButton(Canvas canvas, VButton btn, int alpha) {
        if (btn.bounds.width() <= 0 || btn.bounds.height() <= 0) return;

        RectF r = btn.bounds;
        boolean pressed = btn.pressed;
        float radius = btn.isCircle ? (r.width() * 0.5f) : (5.0f * mDensity);

        // Body fill
        int bodyColor = pressed ? Color.argb(Math.min(255, alpha + 40), 45, 45, 45)
                                : Color.argb(alpha, 70, 70, 70);
        mPaint.setColor(bodyColor);
        mPaint.setStyle(Paint.Style.FILL);
        if (btn.isCircle) {
            canvas.drawCircle(r.centerX(), r.centerY(), r.width() * 0.5f, mPaint);
        } else {
            canvas.drawRoundRect(r, radius, radius, mPaint);
        }

        // 3D Bevel border
        int highColor = pressed ? Color.argb(alpha, 35, 35, 35) : Color.argb(alpha, 160, 160, 160);
        int shadowColor = pressed ? Color.argb(alpha, 160, 160, 160) : Color.argb(alpha, 35, 35, 35);

        mBorderPaint.setStrokeWidth(2.5f * mDensity);
        mBorderPaint.setColor(pressed ? Color.argb(230, 0, 180, 240) : Color.argb(alpha, 25, 25, 25));
        if (btn.isCircle) {
            canvas.drawCircle(r.centerX(), r.centerY(), r.width() * 0.5f, mBorderPaint);
        } else {
            canvas.drawRoundRect(r, radius, radius, mBorderPaint);
        }

        // Render Icon / Glyphs
        int iconColor = pressed ? Color.argb(255, 255, 230, 0) : Color.argb(240, 240, 240, 240);
        float cx = r.centerX();
        float cy = r.centerY();
        float size = Math.min(r.width(), r.height());

        switch (btn.iconType) {
            case 1: // Arrow Up
                drawArrow(canvas, cx, cy, size * 0.45f, 0, iconColor);
                break;
            case 2: // Arrow Down
                drawArrow(canvas, cx, cy, size * 0.45f, 1, iconColor);
                break;
            case 3: // Arrow Left
                drawArrow(canvas, cx, cy, size * 0.45f, 2, iconColor);
                break;
            case 4: // Arrow Right
                drawArrow(canvas, cx, cy, size * 0.45f, 3, iconColor);
                break;
            case 5: // Sneak Notch
                drawSneakIcon(canvas, cx, cy, size * 0.38f, iconColor);
                break;
            case 6: // Sword (Attack)
                drawSwordIcon(canvas, cx, cy, size * 0.55f, iconColor);
                break;
            case 7: // Hand (Use)
                drawHandIcon(canvas, cx, cy, size * 0.52f, iconColor);
                break;
            default: // Text label
                mTextPaint.setTextSize(r.height() * 0.40f);
                mTextPaint.setColor(iconColor);
                float textY = cy - ((mTextPaint.descent() + mTextPaint.ascent()) / 2);
                canvas.drawText(btn.label, cx, textY, mTextPaint);
                break;
        }
    }

    // Classic Connected Cross D-Pad (Style 1)
    private void drawClassicCrossDpad(Canvas canvas, int alpha) {
        float left = btnLeft.bounds.left;
        float right = btnRight.bounds.right;
        float top = btnUp.bounds.top;
        float bottom = btnDown.bounds.bottom;
        float midLeft = btnCenter.bounds.left;
        float midRight = btnCenter.bounds.right;
        float midTop = btnCenter.bounds.top;
        float midBottom = btnCenter.bounds.bottom;

        // Draw cross background
        mPaint.setColor(Color.argb(alpha, 65, 65, 65));
        mPaint.setStyle(Paint.Style.FILL);
        canvas.drawRect(midLeft, top, midRight, bottom, mPaint); // Vertical arm
        canvas.drawRect(left, midTop, right, midBottom, mPaint); // Horizontal arm

        // Draw outer bevel outline
        mBorderPaint.setStrokeWidth(2.5f * mDensity);
        mBorderPaint.setColor(Color.argb(alpha, 25, 25, 25));
        mPath.reset();
        mPath.moveTo(midLeft, top);
        mPath.lineTo(midRight, top);
        mPath.lineTo(midRight, midTop);
        mPath.lineTo(right, midTop);
        mPath.lineTo(right, midBottom);
        mPath.lineTo(midRight, midBottom);
        mPath.lineTo(midRight, bottom);
        mPath.lineTo(midLeft, bottom);
        mPath.lineTo(midLeft, midBottom);
        mPath.lineTo(left, midBottom);
        mPath.lineTo(left, midTop);
        mPath.lineTo(midLeft, midTop);
        mPath.close();
        canvas.drawPath(mPath, mBorderPaint);

        // Individual arm buttons highlight when pressed
        if (btnUp.pressed) {
            mPaint.setColor(Color.argb(Math.min(255, alpha + 50), 30, 30, 30));
            canvas.drawRect(btnUp.bounds, mPaint);
        }
        if (btnDown.pressed) {
            mPaint.setColor(Color.argb(Math.min(255, alpha + 50), 30, 30, 30));
            canvas.drawRect(btnDown.bounds, mPaint);
        }
        if (btnLeft.pressed) {
            mPaint.setColor(Color.argb(Math.min(255, alpha + 50), 30, 30, 30));
            canvas.drawRect(btnLeft.bounds, mPaint);
        }
        if (btnRight.pressed) {
            mPaint.setColor(Color.argb(Math.min(255, alpha + 50), 30, 30, 30));
            canvas.drawRect(btnRight.bounds, mPaint);
        }

        // Draw Arrows
        float armSize = btnUp.bounds.height() * 0.45f;
        drawArrow(canvas, btnUp.bounds.centerX(), btnUp.bounds.centerY(), armSize, 0, btnUp.pressed ? Color.YELLOW : Color.WHITE);
        drawArrow(canvas, btnDown.bounds.centerX(), btnDown.bounds.centerY(), armSize, 1, btnDown.pressed ? Color.YELLOW : Color.WHITE);
        drawArrow(canvas, btnLeft.bounds.centerX(), btnLeft.bounds.centerY(), armSize, 2, btnLeft.pressed ? Color.YELLOW : Color.WHITE);
        drawArrow(canvas, btnRight.bounds.centerX(), btnRight.bounds.centerY(), armSize, 3, btnRight.pressed ? Color.YELLOW : Color.WHITE);

        // Draw Center Circular Sneak Button
        drawMinecraftButton(canvas, btnCenter, alpha);
    }

    // Analog Joystick (Style 2)
    private void drawJoystick(Canvas canvas, int alpha) {
        // Outer Base Ring
        mPaint.setStyle(Paint.Style.FILL);
        mPaint.setColor(Color.argb((int)(alpha * 0.6f), 40, 40, 40));
        canvas.drawCircle(mJoyCenterX, mJoyCenterY, mJoyBaseRadius, mPaint);

        mBorderPaint.setStrokeWidth(3.0f * mDensity);
        mBorderPaint.setColor(mJoyActive ? Color.argb(alpha, 0, 180, 240) : Color.argb(alpha, 120, 120, 120));
        canvas.drawCircle(mJoyCenterX, mJoyCenterY, mJoyBaseRadius, mBorderPaint);

        // Direction indicators inside base
        mPaint.setColor(Color.argb((int)(alpha * 0.35f), 200, 200, 200));
        float arrowOff = mJoyBaseRadius * 0.72f;
        drawArrow(canvas, mJoyCenterX, mJoyCenterY - arrowOff, 12 * mDensity, 0, Color.argb(alpha, 180, 180, 180));
        drawArrow(canvas, mJoyCenterX, mJoyCenterY + arrowOff, 12 * mDensity, 1, Color.argb(alpha, 180, 180, 180));
        drawArrow(canvas, mJoyCenterX - arrowOff, mJoyCenterY, 12 * mDensity, 2, Color.argb(alpha, 180, 180, 180));
        drawArrow(canvas, mJoyCenterX + arrowOff, mJoyCenterY, 12 * mDensity, 3, Color.argb(alpha, 180, 180, 180));

        // Inner Thumbstick Knob
        mPaint.setColor(mJoyActive ? Color.argb(Math.min(255, alpha + 40), 60, 60, 60) : Color.argb(alpha, 80, 80, 80));
        canvas.drawCircle(mJoyKnobX, mJoyKnobY, mJoyKnobRadius, mPaint);

        mBorderPaint.setStrokeWidth(2.5f * mDensity);
        mBorderPaint.setColor(mJoyActive ? Color.argb(255, 0, 200, 255) : Color.argb(alpha, 180, 180, 180));
        canvas.drawCircle(mJoyKnobX, mJoyKnobY, mJoyKnobRadius, mBorderPaint);

        // Center dot on knob
        mPaint.setColor(mJoyActive ? Color.CYAN : Color.argb(alpha, 220, 220, 220));
        canvas.drawCircle(mJoyKnobX, mJoyKnobY, 5 * mDensity, mPaint);
    }

    // Directional Arrow Helper
    private void drawArrow(Canvas canvas, float cx, float cy, float size, int direction, int color) {
        mPaint.setColor(color);
        mPaint.setStyle(Paint.Style.FILL);
        mPath.reset();

        float half = size * 0.5f;
        switch (direction) {
            case 0: // UP
                mPath.moveTo(cx, cy - half);
                mPath.lineTo(cx + half, cy + half);
                mPath.lineTo(cx - half, cy + half);
                break;
            case 1: // DOWN
                mPath.moveTo(cx, cy + half);
                mPath.lineTo(cx + half, cy - half);
                mPath.lineTo(cx - half, cy - half);
                break;
            case 2: // LEFT
                mPath.moveTo(cx - half, cy);
                mPath.lineTo(cx + half, cy - half);
                mPath.lineTo(cx + half, cy + half);
                break;
            case 3: // RIGHT
                mPath.moveTo(cx + half, cy);
                mPath.lineTo(cx - half, cy - half);
                mPath.lineTo(cx - half, cy + half);
                break;
        }
        mPath.close();
        canvas.drawPath(mPath, mPaint);
    }

    // Sneak Notch Icon (Minecraft Bedrock style)
    private void drawSneakIcon(Canvas canvas, float cx, float cy, float size, int color) {
        mPaint.setColor(color);
        mPaint.setStyle(Paint.Style.FILL);
        mPath.reset();
        float w = size * 0.7f;
        float h = size * 0.25f;
        // Diamond / notch shape
        mPath.moveTo(cx - w, cy - h);
        mPath.lineTo(cx + w, cy - h);
        mPath.lineTo(cx + w * 0.7f, cy);
        mPath.lineTo(cx, cy + h * 1.6f);
        mPath.lineTo(cx - w * 0.7f, cy);
        mPath.close();
        canvas.drawPath(mPath, mPaint);
    }

    // Minecraft Sword Icon
    private void drawSwordIcon(Canvas canvas, float cx, float cy, float size, int color) {
        mPaint.setColor(color);
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeWidth(3.0f * mDensity);
        mPaint.setStrokeCap(Paint.Cap.ROUND);

        float s = size * 0.45f;
        // Blade (diagonal line)
        canvas.drawLine(cx - s * 0.6f, cy + s * 0.6f, cx + s * 0.8f, cy - s * 0.8f, mPaint);

        // Guard cross
        mPaint.setStrokeWidth(2.5f * mDensity);
        canvas.drawLine(cx - s * 0.4f, cy + s * 0.1f, cx - s * 0.1f, cy + s * 0.4f, mPaint);

        // Pommel
        canvas.drawCircle(cx - s * 0.75f, cy + s * 0.75f, 2.0f * mDensity, mPaint);
        mPaint.setStyle(Paint.Style.FILL);
    }

    // Minecraft Hand / Pointer Icon
    private void drawHandIcon(Canvas canvas, float cx, float cy, float size, int color) {
        mPaint.setColor(color);
        mPaint.setStyle(Paint.Style.FILL);
        mPath.reset();

        float s = size * 0.45f;
        // Finger pointing up-right
        mPath.moveTo(cx - s * 0.5f, cy + s * 0.6f);
        mPath.lineTo(cx - s * 0.5f, cy - s * 0.1f);
        mPath.lineTo(cx - s * 0.1f, cy - s * 0.7f);
        mPath.lineTo(cx + s * 0.2f, cy - s * 0.7f);
        mPath.lineTo(cx + s * 0.2f, cy + s * 0.1f);
        mPath.lineTo(cx + s * 0.5f, cy + s * 0.1f);
        mPath.lineTo(cx + s * 0.5f, cy + s * 0.6f);
        mPath.close();
        canvas.drawPath(mPath, mPaint);
    }

    private float mDownX = 0;
    private float mDownY = 0;
    private boolean mIsDragging = false;
    private long mMenuTouchDownTime = 0;
    private float mLastMenuTouchX = 0;
    private float mLastMenuTouchY = 0;
    private final Runnable mReleaseMenuTouch = new Runnable() {
        @Override
        public void run() {
            SDLActivity.onNativeMouse(0, MotionEvent.ACTION_UP, mLastMenuTouchX, mLastMenuTouchY, false);
        }
    };

    private void pressButton(VButton btn, int pointerId) {
        btn.pressed = true;
        btn.pointerId = pointerId;
        if (btn == btnMine) {
            SDLActivity.onNativeMouse(1, MotionEvent.ACTION_DOWN, 0, 0, false);
        } else if (btn == btnUse) {
            SDLActivity.onNativeMouse(2, MotionEvent.ACTION_DOWN, 0, 0, false);
        } else if (btn.keyCode != 0) {
            SDLActivity.onNativeKeyDown(btn.keyCode);
        }
    }

    private void releaseButton(VButton btn) {
        btn.pressed = false;
        btn.pointerId = -1;
        if (btn == btnMine || btn == btnUse) {
            SDLActivity.onNativeMouse(0, MotionEvent.ACTION_UP, 0, 0, false);
        } else if (btn.keyCode != 0) {
            SDLActivity.onNativeKeyUp(btn.keyCode);
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = event.getActionMasked();
        int actionIndex = event.getActionIndex();
        int pointerId = event.getPointerId(actionIndex);
        float x = event.getX(actionIndex);
        float y = event.getY(actionIndex);

        // Always check the 🎮 toggle button first
        if (btnToggleUI.bounds.contains(x, y)) {
            if (action == MotionEvent.ACTION_DOWN || action == MotionEvent.ACTION_POINTER_DOWN) {
                mManualForceVisible = !mManualForceVisible;
                mControlsVisible = mManualForceVisible || mInGameRelativeMouse;
                invalidate();
            }
            return true;
        }

        if (!mControlsVisible) {
            // Direct touch in menu: translate to native mouse clicks
            switch (action) {
                case MotionEvent.ACTION_DOWN:
                case MotionEvent.ACTION_POINTER_DOWN:
                    removeCallbacks(mReleaseMenuTouch);
                    mMenuTouchDownTime = SystemClock.uptimeMillis();
                    mLastMenuTouchX = x;
                    mLastMenuTouchY = y;
                    SDLActivity.onNativeMouse(1, MotionEvent.ACTION_DOWN, x, y, false);
                    return true;
                case MotionEvent.ACTION_MOVE:
                    mLastMenuTouchX = x;
                    mLastMenuTouchY = y;
                    for (int i = 0; i < event.getPointerCount(); i++) {
                        SDLActivity.onNativeMouse(1, MotionEvent.ACTION_MOVE, event.getX(i), event.getY(i), false);
                    }
                    return true;
                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_POINTER_UP:
                case MotionEvent.ACTION_CANCEL:
                    mLastMenuTouchX = x;
                    mLastMenuTouchY = y;
                    long elapsed = SystemClock.uptimeMillis() - mMenuTouchDownTime;
                    if (elapsed < 80) {
                        postDelayed(mReleaseMenuTouch, 80 - elapsed);
                    } else {
                        mReleaseMenuTouch.run();
                    }
                    return true;
            }
            return true;
        }

        switch (action) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN: {
                // Mode 2 Joystick touch capture
                if (mControlStyle == 2 && mJoyPointerId == -1) {
                    float dist = (float) Math.hypot(x - mJoyCenterX, y - mJoyCenterY);
                    if (dist <= mJoyBaseRadius * 1.35f) {
                        mJoyPointerId = pointerId;
                        mJoyActive = true;
                        updateJoystickInput(x, y);
                        invalidate();
                        return true;
                    }
                }

                // Check Button hits
                VButton hit = findButton(x, y);
                if (hit != null && hit != btnToggleUI) {
                    pressButton(hit, pointerId);
                    invalidate();
                    return true;
                }

                mDownX = x;
                mDownY = y;
                mIsDragging = false;
                break;
            }

            case MotionEvent.ACTION_MOVE: {
                // Update Joystick if active
                if (mControlStyle == 2 && mJoyActive) {
                    for (int i = 0; i < event.getPointerCount(); i++) {
                        if (event.getPointerId(i) == mJoyPointerId) {
                            updateJoystickInput(event.getX(i), event.getY(i));
                            break;
                        }
                    }
                }

                // Update Buttons
                for (int i = 0; i < event.getPointerCount(); i++) {
                    int pId = event.getPointerId(i);
                    float curX = event.getX(i);
                    float curY = event.getY(i);

                    for (VButton btn : allButtons) {
                        if (btn.pointerId == pId) {
                            boolean stillInside = btn.bounds.contains(curX, curY);
                            if (!stillInside && btn.pressed) {
                                releaseButton(btn);
                                invalidate();
                            }
                        }
                    }
                }

                for (int i = 0; i < event.getPointerCount(); i++) {
                    float curX = event.getX(i);
                    float curY = event.getY(i);
                    if (Math.hypot(curX - mDownX, curY - mDownY) > 10 * mDensity) {
                        mIsDragging = true;
                    }
                }
                break;
            }

            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
            case MotionEvent.ACTION_CANCEL: {
                // Release Joystick if lifted
                if (mControlStyle == 2 && pointerId == mJoyPointerId) {
                    resetJoystick();
                    invalidate();
                    return true;
                }

                boolean buttonHit = false;
                for (VButton btn : allButtons) {
                    if (btn.pointerId == pointerId) {
                        releaseButton(btn);
                        invalidate();
                        buttonHit = true;
                    }
                }
                if (buttonHit) {
                    return true;
                }
                break;
            }
        }

        // Pass unhandled touches (camera rotation, GUI clicks) directly to SDL Surface
        if (mSurface != null) {
            return mSurface.dispatchTouchEvent(event);
        }
        return super.onTouchEvent(event);
    }

    private void updateJoystickInput(float touchX, float touchY) {
        float dx = touchX - mJoyCenterX;
        float dy = touchY - mJoyCenterY;
        float dist = (float) Math.hypot(dx, dy);

        if (dist > mJoyBaseRadius) {
            dx = dx * (mJoyBaseRadius / dist);
            dy = dy * (mJoyBaseRadius / dist);
        }

        mJoyKnobX = mJoyCenterX + dx;
        mJoyKnobY = mJoyCenterY + dy;

        float deadzone = mJoyBaseRadius * 0.22f;

        // Up (W)
        boolean up = dy < -deadzone;
        if (up != mJoyUpPressed) {
            mJoyUpPressed = up;
            if (up) SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_W);
            else SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_W);
        }

        // Down (S)
        boolean down = dy > deadzone;
        if (down != mJoyDownPressed) {
            mJoyDownPressed = down;
            if (down) SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_S);
            else SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_S);
        }

        // Left (A)
        boolean left = dx < -deadzone;
        if (left != mJoyLeftPressed) {
            mJoyLeftPressed = left;
            if (left) SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_A);
            else SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_A);
        }

        // Right (D)
        boolean right = dx > deadzone;
        if (right != mJoyRightPressed) {
            mJoyRightPressed = right;
            if (right) SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_D);
            else SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_D);
        }

        invalidate();
    }

    private void resetJoystick() {
        mJoyActive = false;
        mJoyPointerId = -1;
        mJoyKnobX = mJoyCenterX;
        mJoyKnobY = mJoyCenterY;

        if (mJoyUpPressed) {
            mJoyUpPressed = false;
            SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_W);
        }
        if (mJoyDownPressed) {
            mJoyDownPressed = false;
            SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_S);
        }
        if (mJoyLeftPressed) {
            mJoyLeftPressed = false;
            SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_A);
        }
        if (mJoyRightPressed) {
            mJoyRightPressed = false;
            SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_D);
        }
    }

    private VButton findButton(float x, float y) {
        for (VButton btn : allButtons) {
            if (btn.bounds.width() > 0 && btn.bounds.contains(x, y)) {
                return btn;
            }
        }
        return null;
    }
}
