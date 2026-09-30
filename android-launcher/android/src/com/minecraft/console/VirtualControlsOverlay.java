package com.minecraft.console;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.os.SystemClock;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import org.libsdl.app.SDLActivity;
import org.libsdl.app.SDLSurface;

import java.util.HashMap;
import java.util.Map;

public class VirtualControlsOverlay extends View {

    private final SDLSurface mSurface;
    private final Paint mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Paint mTextPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Paint mFpsPaint = new Paint(Paint.ANTI_ALIAS_FLAG);

    // Button states
    public static class VButton {
        String label;
        int keyCode;
        RectF bounds = new RectF();
        boolean pressed = false;
        int pointerId = -1;
        int color = Color.argb(120, 40, 40, 40);
        int pressedColor = Color.argb(200, 0, 160, 230);
        boolean isToggle = false;

        VButton(String label, int keyCode) {
            this.label = label;
            this.keyCode = keyCode;
        }
    }

    private final VButton btnUp = new VButton("▲", KeyEvent.KEYCODE_W);
    private final VButton btnDown = new VButton("▼", KeyEvent.KEYCODE_S);
    private final VButton btnLeft = new VButton("◀", KeyEvent.KEYCODE_A);
    private final VButton btnRight = new VButton("▶", KeyEvent.KEYCODE_D);
    private final VButton btnCenter = new VButton("◆", KeyEvent.KEYCODE_SHIFT_LEFT); // Sneak

    private final VButton btnJump = new VButton("JUMP", KeyEvent.KEYCODE_SPACE);
    private final VButton btnMine = new VButton("MINE", KeyEvent.KEYCODE_ENTER); // Attack / Break
    private final VButton btnUse = new VButton("USE", KeyEvent.KEYCODE_F);       // Place / Interact
    private final VButton btnInv = new VButton("INV", KeyEvent.KEYCODE_E);       // Inventory
    private final VButton btnDrop = new VButton("DROP", KeyEvent.KEYCODE_Q);     // Drop item

    private final VButton btnPause = new VButton("II", KeyEvent.KEYCODE_ESCAPE);  // Pause
    private final VButton btnF5 = new VButton("F5", KeyEvent.KEYCODE_F5);        // Perspective
    private final VButton btnToggleUI = new VButton("🎮", 0);                   // Toggle overlay visibility

    private final VButton[] allButtons = new VButton[] {
        btnUp, btnDown, btnLeft, btnRight, btnCenter,
        btnJump, btnMine, btnUse, btnInv, btnDrop,
        btnPause, btnF5, btnToggleUI
    };

    private boolean mControlsVisible = false;
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
                        mControlsVisible = enabled;
                        invalidate();
                    }
                });
            }
        };
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        layoutButtons(w, h);
    }

    private void layoutButtons(int w, int h) {
        float d = mDensity;

        // D-Pad on bottom-left
        float padSize = 58 * d;
        float padLeft = 28 * d;
        float padBottom = h - 28 * d;
        float dpadCenterX = padLeft + padSize * 1.5f;
        float dpadCenterY = padBottom - padSize * 1.5f;

        btnCenter.bounds.set(dpadCenterX - padSize * 0.5f, dpadCenterY - padSize * 0.5f,
                             dpadCenterX + padSize * 0.5f, dpadCenterY + padSize * 0.5f);
        btnUp.bounds.set(dpadCenterX - padSize * 0.5f, dpadCenterY - padSize * 1.5f,
                         dpadCenterX + padSize * 0.5f, dpadCenterY - padSize * 0.5f);
        btnDown.bounds.set(dpadCenterX - padSize * 0.5f, dpadCenterY + padSize * 0.5f,
                           dpadCenterX + padSize * 0.5f, dpadCenterY + padSize * 1.5f);
        btnLeft.bounds.set(dpadCenterX - padSize * 1.5f, dpadCenterY - padSize * 0.5f,
                          dpadCenterX - padSize * 0.5f, dpadCenterY + padSize * 0.5f);
        btnRight.bounds.set(dpadCenterX + padSize * 0.5f, dpadCenterY - padSize * 0.5f,
                           dpadCenterX + padSize * 1.5f, dpadCenterY + padSize * 0.5f);

        // Action buttons on bottom-right
        float rightMargin = w - 24 * d;
        float btnSize = 56 * d;

        // Jump button (big, bottom right)
        float jumpX = rightMargin - btnSize * 1.6f;
        float jumpY = h - 30 * d - btnSize * 1.6f;
        btnJump.bounds.set(jumpX, jumpY, jumpX + btnSize * 1.4f, jumpY + btnSize * 1.4f);

        // Mine button (attack) - above Jump
        float mineX = jumpX - btnSize * 1.1f;
        float mineY = jumpY - btnSize * 0.6f;
        btnMine.bounds.set(mineX, mineY, mineX + btnSize, mineY + btnSize);

        // Use button (interact / place) - right of Mine
        float useX = jumpX;
        float useY = jumpY - btnSize * 1.1f;
        btnUse.bounds.set(useX, useY, useX + btnSize, useY + btnSize);

        // Inventory button
        float invX = mineX - btnSize * 0.8f;
        float invY = jumpY + btnSize * 0.2f;
        btnInv.bounds.set(invX, invY, invX + btnSize * 0.8f, invY + btnSize * 0.8f);

        // Drop button
        float dropX = mineX;
        float dropY = mineY - btnSize * 0.9f;
        btnDrop.bounds.set(dropX, dropY, dropX + btnSize * 0.8f, dropY + btnSize * 0.8f);

        // Top bar buttons
        float topY = 12 * d;
        float topBtnW = 38 * d;
        float topBtnH = 30 * d;

        // Pause top center
        float pauseX = w * 0.5f - topBtnW * 0.5f;
        btnPause.bounds.set(pauseX, topY, pauseX + topBtnW, topY + topBtnH);

        // F5 top center-left
        float f5X = pauseX - topBtnW - 12 * d;
        btnF5.bounds.set(f5X, topY, f5X + topBtnW, topY + topBtnH);

        // Toggle UI button top right
        float togX = w - 48 * d;
        btnToggleUI.bounds.set(togX, topY, togX + topBtnW, topY + topBtnH);

        // FPS Box top left
        mFpsBox.set(14 * d, topY, 14 * d + 84 * d, topY + topBtnH);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

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
        drawButton(canvas, btnToggleUI);

        // Draw virtual controls if enabled
        if (mControlsVisible) {
            for (VButton btn : allButtons) {
                if (btn != btnToggleUI) {
                    drawButton(canvas, btn);
                }
            }
        }

        // Request next frame to keep FPS counter updated
        postInvalidateDelayed(16);
    }

    private void drawButton(Canvas canvas, VButton btn) {
        mPaint.setColor(btn.pressed ? btn.pressedColor : btn.color);
        float radius = (btn == btnJump || btn == btnCenter) ? btn.bounds.width() * 0.5f : 8 * mDensity;
        canvas.drawRoundRect(btn.bounds, radius, radius, mPaint);

        // Border
        mPaint.setColor(btn.pressed ? Color.CYAN : Color.argb(100, 200, 200, 200));
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeWidth(2 * mDensity);
        canvas.drawRoundRect(btn.bounds, radius, radius, mPaint);
        mPaint.setStyle(Paint.Style.FILL);

        // Text
        mTextPaint.setTextSize(btn.bounds.height() * 0.42f);
        mTextPaint.setColor(btn.pressed ? Color.YELLOW : Color.WHITE);
        float textY = btn.bounds.centerY() - ((mTextPaint.descent() + mTextPaint.ascent()) / 2);
        canvas.drawText(btn.label, btn.bounds.centerX(), textY, mTextPaint);
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

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = event.getActionMasked();
        int actionIndex = event.getActionIndex();
        int pointerId = event.getPointerId(actionIndex);

        if (!mControlsVisible) {
            float x = event.getX(actionIndex);
            float y = event.getY(actionIndex);

            // Allow clicking the 🎮 toggle button to show controls
            if (action == MotionEvent.ACTION_DOWN && btnToggleUI.bounds.contains(x, y)) {
                mControlsVisible = true;
                invalidate();
                return true;
            }

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
                float x = event.getX(actionIndex);
                float y = event.getY(actionIndex);

                // Check toggle button first
                if (btnToggleUI.bounds.contains(x, y)) {
                    mControlsVisible = !mControlsVisible;
                    invalidate();
                    return true;
                }

                if (mControlsVisible) {
                    VButton hit = findButton(x, y);
                    if (hit != null) {
                        hit.pressed = true;
                        hit.pointerId = pointerId;
                        if (hit.keyCode != 0) {
                            SDLActivity.onNativeKeyDown(hit.keyCode);
                        }
                        invalidate();
                        return true;
                    }
                }

                mDownX = x;
                mDownY = y;
                mIsDragging = false;
                break;
            }

            case MotionEvent.ACTION_MOVE: {
                boolean anyHandled = false;
                if (mControlsVisible) {
                    for (int i = 0; i < event.getPointerCount(); i++) {
                        int pId = event.getPointerId(i);
                        float x = event.getX(i);
                        float y = event.getY(i);

                        for (VButton btn : allButtons) {
                            if (btn.pointerId == pId) {
                                boolean stillInside = btn.bounds.contains(x, y);
                                if (!stillInside && btn.pressed) {
                                    btn.pressed = false;
                                    btn.pointerId = -1;
                                    if (btn.keyCode != 0) {
                                        SDLActivity.onNativeKeyUp(btn.keyCode);
                                    }
                                    invalidate();
                                }
                                anyHandled = true;
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
                boolean buttonHit = false;
                for (VButton btn : allButtons) {
                    if (btn.pointerId == pointerId) {
                        btn.pressed = false;
                        btn.pointerId = -1;
                        if (btn.keyCode != 0) {
                            SDLActivity.onNativeKeyUp(btn.keyCode);
                        }
                        invalidate();
                        buttonHit = true;
                    }
                }
                if (buttonHit) {
                    return true;
                }

                if (!mIsDragging && action == MotionEvent.ACTION_UP) {
                    // Tap on screen: trigger action click for GUI buttons and world interact
                    SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_ENTER);
                    postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_ENTER);
                        }
                    }, 50);
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

    private VButton findButton(float x, float y) {
        for (VButton btn : allButtons) {
            if (btn.bounds.contains(x, y)) {
                return btn;
            }
        }
        return null;
    }
}
