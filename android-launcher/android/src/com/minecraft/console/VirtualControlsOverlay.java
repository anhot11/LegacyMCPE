package com.minecraft.console;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffColorFilter;
import android.graphics.RectF;
import android.os.Handler;
import android.os.Looper;
import android.os.SystemClock;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import org.libsdl.app.SDLActivity;
import org.libsdl.app.SDLSurface;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

/**
 * Authentic Minecraft Bedrock / Pocket Edition Native Touch Controls Overlay.
 * 
 * - COMPLETELY INVISIBLE (View.GONE) in menus, inventory, pause, and title screens.
 * - Native in-world touch interaction:
 *     * TAP (< 250ms): Place block / Interact (doors, chests, furnaces) / Attack mobs.
 *     * HOLD (> 220ms steady): Continuous mining/breaking of block with crack animation.
 *     * DRAG / SWIPE: Ultra-smooth 3D camera rotation with deadzone and clamped deltas.
 * - Dedicated D-Pad / Joystick for movement (multi-touch with camera and interaction).
 * - Dedicated Jump button and Inventory ("...") button next to hotbar.
 * - ZERO clunky keyboard buttons on screen.
 */
public class VirtualControlsOverlay extends View {

    private final SDLSurface mSurface;
    private final Paint mBitmapPaint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.FILTER_BITMAP_FLAG);
    private final Paint mFpsPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final RectF mFpsBox = new RectF();

    // Settings (loaded dynamically from options.txt)
    private int mControlStyle = 0;   // 0: Modern Bedrock, 1: Classic PE D-Pad, 2: Joystick
    private int mControlScale = 1;   // 0: 80%, 1: 100%, 2: 125%, 3: 150%
    private int mControlOpacity = 1; // 0: 35%, 1: 65%, 2: 90%, 3: 100%

    private long mLastOptionsCheck = 0;
    private long mLastOptionsMtime = 0;

    // Cache of loaded Bitmaps
    private final Map<String, Bitmap> mBitmapCache = new HashMap<String, Bitmap>();

    public static class VButton {
        String name;
        int keyCode;
        RectF bounds = new RectF();
        boolean pressed = false;
        int pointerId = -1;
        Bitmap bmpNormal = null;
        Bitmap bmpActive = null;

        VButton(String name, int keyCode) {
            this.name = name;
            this.keyCode = keyCode;
        }
    }

    // Directional buttons
    private final VButton btnUp = new VButton("dpad_up", KeyEvent.KEYCODE_W);
    private final VButton btnDown = new VButton("dpad_down", KeyEvent.KEYCODE_S);
    private final VButton btnLeft = new VButton("dpad_left", KeyEvent.KEYCODE_A);
    private final VButton btnRight = new VButton("dpad_right", KeyEvent.KEYCODE_D);
    private final VButton btnCenter = new VButton("sneak", KeyEvent.KEYCODE_SHIFT_LEFT);

    // Action buttons (Jump, Sneak, Inventory)
    private final VButton btnJump = new VButton("jump", KeyEvent.KEYCODE_SPACE);
    private final VButton btnSneak = new VButton("sneak", KeyEvent.KEYCODE_SHIFT_LEFT);
    private final VButton btnInv = new VButton("inventory", KeyEvent.KEYCODE_E);

    // Top Bar in-game buttons
    private final VButton btnPause = new VButton("pause", KeyEvent.KEYCODE_ESCAPE);
    private final VButton btnPerspective = new VButton("perspective", KeyEvent.KEYCODE_F5);
    private final VButton btnChat = new VButton("chat", KeyEvent.KEYCODE_T);

    private final VButton[] allButtons = new VButton[] {
        btnUp, btnDown, btnLeft, btnRight, btnCenter,
        btnJump, btnSneak, btnInv,
        btnPause, btnPerspective, btnChat
    };

    // Joystick for Style 2
    private Bitmap mBmpJoyPad = null;
    private Bitmap mBmpJoyStick = null;
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

    // Native World Touch Interaction (Hold to Mine, Tap to Place/Attack, Drag to Look)
    private int mWorldPointerId = -1;
    private float mWorldDownX = 0;
    private float mWorldDownY = 0;
    private float mLastWorldX = 0;
    private float mLastWorldY = 0;
    private long mWorldDownTime = 0;
    private boolean mIsPanning = false;
    private boolean mIsMining = false;

    private final Handler mTouchHandler = new Handler(Looper.getMainLooper());
    private Runnable mHoldToMineRunnable = null;

    // Creative Flight Double-Tap Jump
    private long mLastJumpTapTime = 0;

    // MCPE 0.15 Circular Mining Progress Indicator Paints
    private final Paint mRingBgPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Paint mRingOuterPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Paint mRingProgressPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Paint mRingCenterPaint = new Paint(Paint.ANTI_ALIAS_FLAG);

    // Visibility state
    private boolean mControlsVisible = false;
    private float mDensity = 1.0f;

    // FPS Counter
    private int mFrameCount = 0;
    private long mLastFpsTime = 0;
    private int mCurrentFps = 60;

    public VirtualControlsOverlay(Context context, SDLSurface surface) {
        super(context);
        this.mSurface = surface;
        this.mDensity = context.getResources().getDisplayMetrics().density;
        this.mBitmapPaint.setFilterBitmap(false);

        mFpsPaint.setTextAlign(Paint.Align.LEFT);
        mFpsPaint.setFakeBoldText(true);
        mFpsPaint.setTextSize(13 * mDensity);

        // Configure authentic MCPE 0.15 mining indicator
        mRingBgPaint.setStyle(Paint.Style.FILL);
        mRingBgPaint.setColor(Color.argb(80, 0, 0, 0));

        mRingOuterPaint.setStyle(Paint.Style.STROKE);
        mRingOuterPaint.setStrokeWidth(2.5f * mDensity);
        mRingOuterPaint.setColor(Color.argb(160, 255, 255, 255));

        mRingProgressPaint.setStyle(Paint.Style.STROKE);
        mRingProgressPaint.setStrokeWidth(3.5f * mDensity);
        mRingProgressPaint.setStrokeCap(Paint.Cap.ROUND);
        mRingProgressPaint.setColor(Color.argb(230, 255, 255, 255));

        mRingCenterPaint.setStyle(Paint.Style.FILL);
        mRingCenterPaint.setColor(Color.argb(220, 255, 255, 255));

        setFocusable(false);
        setFocusableInTouchMode(false);

        mControlsVisible = false;
        setVisibility(View.GONE);

        checkAndReloadOptions();
        loadThemeBitmaps();

        SDLActivity.mRelativeMouseCallback = new SDLActivity.RelativeMouseCallback() {
            @Override
            public void onRelativeMouseChanged(final boolean enabled) {
                post(new Runnable() {
                    @Override
                    public void run() {
                        mControlsVisible = enabled;
                        setVisibility(enabled ? View.VISIBLE : View.GONE);
                        if (!enabled) {
                            resetAllInputs();
                        }
                        invalidate();
                    }
                });
            }
        };
    }

    private Bitmap loadBitmapAsset(String path) {
        if (mBitmapCache.containsKey(path)) {
            return mBitmapCache.get(path);
        }
        try {
            InputStream is = getContext().getAssets().open("controls/" + path);
            Bitmap bmp = BitmapFactory.decodeStream(is);
            is.close();
            mBitmapCache.put(path, bmp);
            return bmp;
        } catch (Throwable t) {
            return null;
        }
    }

    private void loadThemeBitmaps() {
        String theme = (mControlStyle == 1) ? "classic" : "modern";

        for (VButton btn : allButtons) {
            btn.bmpNormal = loadBitmapAsset(theme + "/" + btn.name + ".png");
            btn.bmpActive = loadBitmapAsset(theme + "/" + btn.name + "_active.png");
            if (btn.bmpActive == null) {
                btn.bmpActive = btn.bmpNormal;
            }
        }

        mBmpJoyPad = loadBitmapAsset(theme + "/joystick_pad.png");
        mBmpJoyStick = loadBitmapAsset(theme + "/joystick_stick.png");
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

            for (String line : content.split("\\r?\\n")) {
                line = line.trim();
                if (line.startsWith("touchControlStyle:")) {
                    try { style = Integer.parseInt(line.substring("touchControlStyle:".length()).trim()); } catch (Exception ignored) {}
                } else if (line.startsWith("touchControlScale:")) {
                    try { scale = Integer.parseInt(line.substring("touchControlScale:".length()).trim()); } catch (Exception ignored) {}
                } else if (line.startsWith("touchControlOpacity:")) {
                    try { opacity = Integer.parseInt(line.substring("touchControlOpacity:".length()).trim()); } catch (Exception ignored) {}
                }
            }

            boolean styleChanged = (style != mControlStyle);
            mControlStyle = style;
            mControlScale = scale;
            mControlOpacity = opacity;

            if (styleChanged) {
                mBitmapCache.clear();
                loadThemeBitmaps();
            }
            updateButtonPositions();
        } catch (Throwable ignored) {}
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        updateButtonPositions();
    }

    private void updateButtonPositions() {
        float w = getWidth();
        float h = getHeight();
        if (w <= 0 || h <= 0) return;

        float d = mDensity;
        float scale = getScaleFactor();

        // 1. FPS Box (top left)
        mFpsBox.set(10 * d, 10 * d, 68 * d, 30 * d);

        // 2. Top bar buttons (Pause, Perspective, Chat)
        float topBtnSize = 28 * d * scale;
        float topY = 8 * d;

        // Pause button (top right corner)
        float pauseX = w - 12 * d - topBtnSize;
        btnPause.bounds.set(pauseX, topY, pauseX + topBtnSize, topY + topBtnSize);

        // Chat button (next to pause)
        float chatX = pauseX - 8 * d - topBtnSize;
        btnChat.bounds.set(chatX, topY, chatX + topBtnSize, topY + topBtnSize);

        // Perspective button (center-top)
        float perspX = (w - topBtnSize) / 2.0f;
        btnPerspective.bounds.set(perspX, topY, perspX + topBtnSize, topY + topBtnSize);

        // 3. Hotbar calculation for Inventory button placement
        float barHeight = Math.max(48.0f * d, h * 0.15f);
        float barWidth = Math.min(w * 0.65f, 9.0f * 42.0f * d);
        float hotbarRight = (w + barWidth) / 2.0f;
        float hotbarTop = h - barHeight;

        // Inventory "..." button right next to the hotbar (MCPE style)
        float invSize = 34 * d * scale;
        float invX = hotbarRight + 8 * d;
        float invY = hotbarTop + (barHeight - invSize) / 2.0f;
        btnInv.bounds.set(invX, invY, invX + invSize, invY + invSize);

        if (mControlStyle == 0 || mControlStyle == 1) {
            // Style 0 (Modern Bedrock) & Style 1 (Classic PE D-Pad)
            float dpadBtn = 56 * d * scale;
            float leftMargin = 20 * d;
            float bottomMargin = 20 * d;

            float cx = leftMargin + dpadBtn * 1.5f;
            float cy = h - bottomMargin - dpadBtn * 1.5f;

            btnUp.bounds.set(cx - dpadBtn / 2, cy - dpadBtn * 1.5f, cx + dpadBtn / 2, cy - dpadBtn * 0.5f);
            btnDown.bounds.set(cx - dpadBtn / 2, cy + dpadBtn * 0.5f, cx + dpadBtn / 2, cy + dpadBtn * 1.5f);
            btnLeft.bounds.set(cx - dpadBtn * 1.5f, cy - dpadBtn / 2, cx - dpadBtn * 0.5f, cy + dpadBtn / 2);
            btnRight.bounds.set(cx + dpadBtn * 0.5f, cy - dpadBtn / 2, cx + dpadBtn * 1.5f, cy + dpadBtn / 2);
            btnCenter.bounds.set(cx - dpadBtn / 2, cy - dpadBtn / 2, cx + dpadBtn / 2, cy + dpadBtn / 2);

            // Right Action: Circular Jump Button
            float jumpSize = 58 * d * scale;
            float jumpX = w - 24 * d - jumpSize;
            float jumpY = h - 28 * d - jumpSize;
            btnJump.bounds.set(jumpX, jumpY, jumpX + jumpSize, jumpY + jumpSize);

            // Crouch is already in the center of the movement D-pad below forward, so remove the duplicate beside jump
            btnSneak.bounds.set(0, 0, 0, 0);

        } else if (mControlStyle == 2) {
            // Style 2: Floating/Fixed Joystick + Action Buttons
            mJoyBaseRadius = 62 * d * scale;
            mJoyKnobRadius = 28 * d * scale;
            mJoyCenterX = 28 * d + mJoyBaseRadius;
            mJoyCenterY = h - 28 * d - mJoyBaseRadius;
            if (!mJoyActive) {
                mJoyKnobX = mJoyCenterX;
                mJoyKnobY = mJoyCenterY;
            }

            btnUp.bounds.set(0, 0, 0, 0);
            btnDown.bounds.set(0, 0, 0, 0);
            btnLeft.bounds.set(0, 0, 0, 0);
            btnRight.bounds.set(0, 0, 0, 0);
            btnCenter.bounds.set(0, 0, 0, 0);

            float jumpSize = 56 * d * scale;
            float jumpX = w - 24 * d - jumpSize;
            float jumpY = h - 28 * d - jumpSize;
            btnJump.bounds.set(jumpX, jumpY, jumpX + jumpSize, jumpY + jumpSize);

            float sneakSize = 46 * d * scale;
            float sneakX = jumpX + (jumpSize - sneakSize) / 2.0f;
            float sneakY = jumpY - sneakSize * 1.25f;
            btnSneak.bounds.set(sneakX, sneakY, sneakX + sneakSize, sneakY + sneakSize);
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        if (!mControlsVisible) {
            return;
        }

        checkAndReloadOptions();

        // Calculate real FPS (in-game only)
        long now = SystemClock.uptimeMillis();
        mFrameCount++;
        if (now - mLastFpsTime >= 500) {
            mCurrentFps = (int) (mFrameCount * 1000.0f / (now - mLastFpsTime));
            mFrameCount = 0;
            mLastFpsTime = now;
        }

        // Draw In-game FPS Counter
        mFpsPaint.setColor(Color.argb(160, 20, 20, 20));
        canvas.drawRoundRect(mFpsBox, 4 * mDensity, 4 * mDensity, mFpsPaint);
        mFpsPaint.setColor(mCurrentFps >= 45 ? Color.GREEN : (mCurrentFps >= 25 ? Color.YELLOW : Color.RED));
        canvas.drawText("FPS: " + mCurrentFps, mFpsBox.left + 6 * mDensity, mFpsBox.centerY() + 4 * mDensity, mFpsPaint);

        int alpha = getControlAlpha();

        // Top bar buttons
        drawButtonBitmap(canvas, btnPause, alpha);
        drawButtonBitmap(canvas, btnPerspective, alpha);
        drawButtonBitmap(canvas, btnChat, alpha);

        // Inventory button
        drawButtonBitmap(canvas, btnInv, alpha);

        if (mControlStyle == 0 || mControlStyle == 1) {
            // Directional controls
            drawButtonBitmap(canvas, btnUp, alpha);
            drawButtonBitmap(canvas, btnDown, alpha);
            drawButtonBitmap(canvas, btnLeft, alpha);
            drawButtonBitmap(canvas, btnRight, alpha);
            drawButtonBitmap(canvas, btnCenter, alpha);

            // Jump
            drawButtonBitmap(canvas, btnJump, alpha);

        } else if (mControlStyle == 2) {
            // Joystick
            if (mBmpJoyPad != null) {
                mBitmapPaint.setAlpha(alpha);
                RectF padRect = new RectF(mJoyCenterX - mJoyBaseRadius, mJoyCenterY - mJoyBaseRadius,
                                          mJoyCenterX + mJoyBaseRadius, mJoyCenterY + mJoyBaseRadius);
                canvas.drawBitmap(mBmpJoyPad, null, padRect, mBitmapPaint);
            }
            if (mBmpJoyStick != null) {
                mBitmapPaint.setAlpha(Math.min(255, alpha + 30));
                RectF stickRect = new RectF(mJoyKnobX - mJoyKnobRadius, mJoyKnobY - mJoyKnobRadius,
                                            mJoyKnobX + mJoyKnobRadius, mJoyKnobY + mJoyKnobRadius);
                canvas.drawBitmap(mBmpJoyStick, null, stickRect, mBitmapPaint);
            }

            drawButtonBitmap(canvas, btnJump, alpha);
            drawButtonBitmap(canvas, btnSneak, alpha);
        }

        // Draw Authentic MCPE 0.15 Circular Mining Progress Indicator at touched coordinates
        if (mIsMining && mWorldPointerId != -1) {
            float cx = mWorldDownX;
            float cy = mWorldDownY;
            float rOuter = 28 * mDensity;
            float rInner = 26 * mDensity;

            // Semi-transparent dark circular backing
            canvas.drawCircle(cx, cy, rOuter + 3 * mDensity, mRingBgPaint);

            // Outer boundary ring
            canvas.drawCircle(cx, cy, rOuter, mRingOuterPaint);

            // Center target dot
            canvas.drawCircle(cx, cy, 3.5f * mDensity, mRingCenterPaint);

            // Progress arc filling clockwise
            long holdMs = Math.max(0, SystemClock.uptimeMillis() - (mWorldDownTime + 200));
            float sweep = Math.min(360.0f, (holdMs % 1200) / 1200.0f * 360.0f);
            RectF arcRect = new RectF(cx - rInner, cy - rInner, cx + rInner, cy + rInner);
            canvas.drawArc(arcRect, -90, sweep, false, mRingProgressPaint);
        }

        postInvalidateDelayed(16);
    }

    private void drawButtonBitmap(Canvas canvas, VButton btn, int alpha) {
        if (btn.bounds.width() <= 0 || btn.bounds.height() <= 0) return;

        Bitmap b = btn.pressed ? btn.bmpActive : btn.bmpNormal;
        if (b != null) {
            mBitmapPaint.setAlpha(alpha);
            if (btn.pressed && btn.bmpActive == btn.bmpNormal) {
                mBitmapPaint.setColorFilter(new PorterDuffColorFilter(Color.argb(80, 0, 180, 255), PorterDuff.Mode.SRC_ATOP));
            } else {
                mBitmapPaint.setColorFilter(null);
            }
            canvas.drawBitmap(b, null, btn.bounds, mBitmapPaint);
        }
    }

    private VButton findButton(float x, float y) {
        for (VButton btn : allButtons) {
            if (btn.bounds.width() > 0) {
                float pad = (btn == btnInv || btn == btnPause || btn == btnPerspective || btn == btnChat) ? 20.0f * mDensity : 0.0f;
                if (x >= btn.bounds.left - pad && x <= btn.bounds.right + pad &&
                    y >= btn.bounds.top - pad && y <= btn.bounds.bottom + pad) {
                    return btn;
                }
            }
        }
        return null;
    }

    private boolean checkHotbarTap(float x, float y) {
        float w = getWidth();
        float h = getHeight();
        if (w <= 0 || h <= 0) return false;

        float barHeight = Math.max(48.0f * mDensity, h * 0.15f);
        float barWidth = Math.min(w * 0.65f, 9.0f * 42.0f * mDensity);
        float left = (w - barWidth) / 2.0f;
        float right = (w + barWidth) / 2.0f;
        float top = h - barHeight;

        if (y >= top && y <= h && x >= left && x <= right) {
            float slotWidth = barWidth / 9.0f;
            int slot = (int) ((x - left) / slotWidth);
            if (slot >= 0 && slot < 9) {
                int key = KeyEvent.KEYCODE_1 + slot;
                SDLActivity.onNativeKeyDown(key);
                SDLActivity.onNativeKeyUp(key);
                return true;
            }
        }
        return false;
    }

    private void pressButton(VButton btn, int pointerId) {
        btn.pressed = true;
        btn.pointerId = pointerId;
        if (btn.keyCode != 0) {
            SDLActivity.onNativeKeyDown(btn.keyCode);
        }
    }

    private void releaseButton(VButton btn) {
        btn.pressed = false;
        btn.pointerId = -1;
        if (btn.keyCode != 0) {
            SDLActivity.onNativeKeyUp(btn.keyCode);
        }
    }

    private void resetAllInputs() {
        for (VButton btn : allButtons) {
            if (btn.pressed) {
                releaseButton(btn);
            }
        }
        if (mJoyActive) {
            resetJoystick();
        }
        cancelWorldInteraction();
    }

    private void cancelWorldInteraction() {
        if (mHoldToMineRunnable != null) {
            mTouchHandler.removeCallbacks(mHoldToMineRunnable);
            mHoldToMineRunnable = null;
        }
        if (mIsMining) {
            SDLActivity.onNativeMouse(0, MotionEvent.ACTION_UP, 0, 0, false);
            mIsMining = false;
        }
        mWorldPointerId = -1;
        mIsPanning = false;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (!mControlsVisible) {
            return false;
        }

        int action = event.getActionMasked();
        int actionIndex = event.getActionIndex();
        int pointerId = event.getPointerId(actionIndex);
        float x = event.getX(actionIndex);
        float y = event.getY(actionIndex);

        switch (action) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN: {
                // Check Joystick in Style 2
                if (mControlStyle == 2 && mJoyPointerId == -1) {
                    float dist = (float) Math.hypot(x - mJoyCenterX, y - mJoyCenterY);
                    if (dist <= mJoyBaseRadius * 1.4f) {
                        mJoyPointerId = pointerId;
                        mJoyActive = true;
                        updateJoystickInput(x, y);
                        invalidate();
                        return true;
                    }
                }

                // Check Button hits (D-Pad, Jump, Sneak, Inv, Top Bar)
                VButton hit = findButton(x, y);
                if (hit != null) {
                    if (hit == btnInv) {
                        btnInv.pressed = true;
                        invalidate();
                        SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_E);
                        postDelayed(new Runnable() {
                            @Override
                            public void run() {
                                SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_E);
                                btnInv.pressed = false;
                                invalidate();
                            }
                        }, 50);
                        return true;
                    }

                    // Top Bar buttons: instant pulse ensures menu/chat/perspective opens without lag or stuck state
                    if (hit == btnPause || hit == btnPerspective || hit == btnChat) {
                        hit.pressed = true;
                        invalidate();
                        SDLActivity.onNativeKeyDown(hit.keyCode);
                        final VButton targetHit = hit;
                        postDelayed(new Runnable() {
                            @Override
                            public void run() {
                                SDLActivity.onNativeKeyUp(targetHit.keyCode);
                                targetHit.pressed = false;
                                invalidate();
                            }
                        }, 50);
                        return true;
                    }

                    // Creative Flight Double-Tap Jump
                    if (hit == btnJump) {
                        long nowTap = SystemClock.uptimeMillis();
                        mLastJumpTapTime = nowTap;
                    }

                    pressButton(hit, pointerId);
                    invalidate();
                    return true;
                }

                // Check in-game hotbar slot tap
                if (checkHotbarTap(x, y)) {
                    return true;
                }

                // Native In-World Touch Interaction (Hold to Mine, Tap to Place/Attack, Drag to Look)
                if (mWorldPointerId == -1) {
                    mWorldPointerId = pointerId;
                    mWorldDownX = x;
                    mWorldDownY = y;
                    mLastWorldX = x;
                    mLastWorldY = y;
                    mWorldDownTime = SystemClock.uptimeMillis();
                    mIsPanning = false;
                    mIsMining = false;

                    // Immediately update native touch coordinates for block selection / raycasting
                    SDLActivity.onNativeMouse(0, MotionEvent.ACTION_HOVER_MOVE, mWorldDownX, mWorldDownY, false);

                    // Schedule Hold to Mine after 200ms steady hold
                    mHoldToMineRunnable = new Runnable() {
                        @Override
                        public void run() {
                            if (mWorldPointerId != -1 && !mIsPanning) {
                                mIsMining = true;
                                // Start continuous block mining at the touched coordinate!
                                SDLActivity.onNativeMouse(1, MotionEvent.ACTION_DOWN, mWorldDownX, mWorldDownY, false);
                            }
                        }
                    };
                    mTouchHandler.postDelayed(mHoldToMineRunnable, 200);
                    return true;
                }
                break;
            }

            case MotionEvent.ACTION_MOVE: {
                // 1. Update Joystick
                if (mControlStyle == 2 && mJoyActive) {
                    for (int i = 0; i < event.getPointerCount(); i++) {
                        if (event.getPointerId(i) == mJoyPointerId) {
                            updateJoystickInput(event.getX(i), event.getY(i));
                            break;
                        }
                    }
                }

                // 2. Update Virtual Buttons
                for (int i = 0; i < event.getPointerCount(); i++) {
                    int pId = event.getPointerId(i);
                    float curX = event.getX(i);
                    float curY = event.getY(i);

                    for (VButton btn : allButtons) {
                        if (btn.pointerId == pId) {
                            if (!btn.bounds.contains(curX, curY) && btn.pressed) {
                                releaseButton(btn);
                                invalidate();
                            }
                        }
                    }
                }

                // 3. Update In-World Touch Interaction (Pan Look vs Hold Mine)
                if (mWorldPointerId != -1) {
                    for (int i = 0; i < event.getPointerCount(); i++) {
                        if (event.getPointerId(i) == mWorldPointerId) {
                            float curX = event.getX(i);
                            float curY = event.getY(i);

                            float distFromDown = (float) Math.hypot(curX - mWorldDownX, curY - mWorldDownY);

                            if (mIsMining) {
                                // While actively mining a block, lock camera completely!
                                if (distFromDown > 45.0f * mDensity) {
                                    SDLActivity.onNativeMouse(0, MotionEvent.ACTION_UP, 0, 0, false);
                                    mIsMining = false;
                                    mIsPanning = true;
                                    mLastWorldX = curX;
                                    mLastWorldY = curY;
                                }
                                break; // Stop here: never pan or move camera while mining!
                            } else if (!mIsPanning) {
                                if (distFromDown > 20.0f * mDensity) {
                                    // Finger moved before hold time -> User is rotating camera
                                    if (mHoldToMineRunnable != null) {
                                        mTouchHandler.removeCallbacks(mHoldToMineRunnable);
                                        mHoldToMineRunnable = null;
                                    }
                                    mIsPanning = true;
                                    mLastWorldX = curX;
                                    mLastWorldY = curY;
                                }
                            }

                            if (mIsPanning) {
                                float dx = curX - mLastWorldX;
                                float dy = curY - mLastWorldY;
                                mLastWorldX = curX;
                                mLastWorldY = curY;

                                // Clamp per-move delta to prevent camera jumping to sky
                                float maxDelta = 150.0f * mDensity;
                                dx = Math.max(-maxDelta, Math.min(maxDelta, dx));
                                dy = Math.max(-maxDelta, Math.min(maxDelta, dy));

                                SDLActivity.onNativeMouse(0, MotionEvent.ACTION_MOVE, dx, dy, true);
                            }
                            break;
                        }
                    }
                }
                break;
            }

            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
            case MotionEvent.ACTION_CANCEL: {
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

                if (pointerId == mWorldPointerId) {
                    if (mHoldToMineRunnable != null) {
                        mTouchHandler.removeCallbacks(mHoldToMineRunnable);
                        mHoldToMineRunnable = null;
                    }

                    if (mIsMining) {
                        // Finished mining
                        SDLActivity.onNativeMouse(0, MotionEvent.ACTION_UP, 0, 0, false);
                        mIsMining = false;
                    } else if (!mIsPanning && (SystemClock.uptimeMillis() - mWorldDownTime) < 250) {
                        // TAP DETECTED: Place block / Interact / Attack targeted entity at touched location!
                        SDLActivity.onNativeMouse(2, MotionEvent.ACTION_DOWN, mWorldDownX, mWorldDownY, false);
                        postDelayed(new Runnable() {
                            @Override
                            public void run() {
                                SDLActivity.onNativeMouse(0, MotionEvent.ACTION_UP, 0, 0, false);
                            }
                        }, 40);
                    }

                    mWorldPointerId = -1;
                    mIsPanning = false;
                    return true;
                }
                break;
            }
        }

        return true;
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

        if (mJoyUpPressed) { mJoyUpPressed = false; SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_W); }
        if (mJoyDownPressed) { mJoyDownPressed = false; SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_S); }
        if (mJoyLeftPressed) { mJoyLeftPressed = false; SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_A); }
        if (mJoyRightPressed) { mJoyRightPressed = false; SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_D); }
    }
}
