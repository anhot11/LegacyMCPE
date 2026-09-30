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
 * - ZERO emulator buttons or overlays in menus. Direct native touch interaction.
 * - Automatically fades in / appears ONLY during active 3D gameplay (relative mouse mode).
 * - Renders official Minecraft Bedrock and Classic PE textures with real pressed states (_active.png).
 * - Smooth camera look and multi-touch support.
 */
public class VirtualControlsOverlay extends View {

    private final SDLSurface mSurface;
    private final Paint mBitmapPaint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.FILTER_BITMAP_FLAG);
    private final Paint mFpsPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final RectF mFpsBox = new RectF();

    // Settings (loaded dynamically from options.txt)
    // touchControlStyle: 0: Modern Bedrock, 1: Classic PE D-Pad, 2: Joystick + Action
    private int mControlStyle = 0;
    // touchControlScale: 0: 80%, 1: 100%, 2: 125%, 3: 150%
    private int mControlScale = 1;
    // touchControlOpacity: 0: 35%, 1: 65%, 2: 90%, 3: 100%
    private int mControlOpacity = 1;

    private long mLastOptionsCheck = 0;
    private long mLastOptionsMtime = 0;

    // Cache of loaded Bitmaps
    private final Map<String, Bitmap> mBitmapCache = new HashMap<String, Bitmap>();

    public static class VButton {
        String name;
        int keyCode;
        int mouseButton; // 1: Attack/Mine, 2: Interact/Use, 0: Key
        RectF bounds = new RectF();
        boolean pressed = false;
        int pointerId = -1;
        Bitmap bmpNormal = null;
        Bitmap bmpActive = null;

        VButton(String name, int keyCode, int mouseButton) {
            this.name = name;
            this.keyCode = keyCode;
            this.mouseButton = mouseButton;
        }
    }

    // Directional buttons
    private final VButton btnUp = new VButton("dpad_up", KeyEvent.KEYCODE_W, 0);
    private final VButton btnDown = new VButton("dpad_down", KeyEvent.KEYCODE_S, 0);
    private final VButton btnLeft = new VButton("dpad_left", KeyEvent.KEYCODE_A, 0);
    private final VButton btnRight = new VButton("dpad_right", KeyEvent.KEYCODE_D, 0);
    private final VButton btnCenter = new VButton("sneak", KeyEvent.KEYCODE_SHIFT_LEFT, 0);

    // Action buttons
    private final VButton btnJump = new VButton("jump", KeyEvent.KEYCODE_SPACE, 0);
    private final VButton btnMine = new VButton("attack", 0, 1);
    private final VButton btnUse = new VButton("interact", 0, 2);
    private final VButton btnInv = new VButton("inventory", KeyEvent.KEYCODE_E, 0);
    private final VButton btnSneak = new VButton("sneak", KeyEvent.KEYCODE_SHIFT_LEFT, 0);

    // Top Bar in-game buttons
    private final VButton btnPause = new VButton("pause", KeyEvent.KEYCODE_ESCAPE, 0);
    private final VButton btnPerspective = new VButton("perspective", KeyEvent.KEYCODE_F5, 0);
    private final VButton btnChat = new VButton("chat", KeyEvent.KEYCODE_T, 0);

    private final VButton[] allButtons = new VButton[] {
        btnUp, btnDown, btnLeft, btnRight, btnCenter,
        btnJump, btnMine, btnUse, btnInv, btnSneak,
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

    // Camera Look tracking
    private int mCameraPointerId = -1;
    private float mLastCameraX = 0;
    private float mLastCameraY = 0;

    // Visibility state: MUST be false in menus!
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

        setFocusable(false);
        setFocusableInTouchMode(false);

        // By default on startup, we are in the TitleScreen / Launcher -> COMPLETELY HIDDEN
        mControlsVisible = false;
        setVisibility(View.GONE);

        checkAndReloadOptions();
        loadThemeBitmaps();

        // Listen to SDL relative mouse mode changes
        // When in-game (relative mouse mode enabled), show controls
        // When in menus / pause / inventories (relative mouse mode disabled), hide controls completely!
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
                        loadThemeBitmaps();
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

        // Top bar buttons (In-game only): Pause, F5, Chat
        float topY = 10 * d;
        float topSize = 34 * d;

        float pauseX = w * 0.5f - topSize * 0.5f;
        btnPause.bounds.set(pauseX, topY, pauseX + topSize, topY + topSize);

        float f5X = pauseX - topSize - 14 * d;
        btnPerspective.bounds.set(f5X, topY, f5X + topSize, topY + topSize);

        float chatX = pauseX + topSize + 14 * d;
        btnChat.bounds.set(chatX, topY, chatX + topSize, topY + topSize);

        mFpsBox.set(12 * d, topY, 12 * d + 72 * d, topY + topSize);

        if (mControlStyle == 0 || mControlStyle == 1) {
            // Style 0 (Modern Bedrock - Separated) & Style 1 (Classic PE - Connected Cross)
            float padSize = (mControlStyle == 0 ? 56 : 52) * d * scale;
            float padLeft = 24 * d;
            float padBottom = h - 24 * d;
            float dpadCenterX = padLeft + padSize * 1.5f;
            float dpadCenterY = padBottom - padSize * 1.5f;

            float gap = (mControlStyle == 0) ? (3 * d * scale) : 0;

            btnCenter.bounds.set(dpadCenterX - padSize * 0.5f, dpadCenterY - padSize * 0.5f,
                                 dpadCenterX + padSize * 0.5f, dpadCenterY + padSize * 0.5f);

            btnUp.bounds.set(dpadCenterX - padSize * 0.5f, dpadCenterY - padSize * 1.5f - gap,
                             dpadCenterX + padSize * 0.5f, dpadCenterY - padSize * 0.5f - gap);
            btnDown.bounds.set(dpadCenterX - padSize * 0.5f, dpadCenterY + padSize * 0.5f + gap,
                               dpadCenterX + padSize * 0.5f, dpadCenterY + padSize * 1.5f + gap);
            btnLeft.bounds.set(dpadCenterX - padSize * 1.5f - gap, dpadCenterY - padSize * 0.5f,
                              dpadCenterX - padSize * 0.5f - gap, dpadCenterY + padSize * 0.5f);
            btnRight.bounds.set(dpadCenterX + padSize * 0.5f + gap, dpadCenterY - padSize * 0.5f,
                               dpadCenterX + padSize * 1.5f + gap, dpadCenterY + padSize * 0.5f);

            // Right side Action buttons
            float rightMargin = w - 24 * d;
            float btnSize = 58 * d * scale;

            // Jump button (large)
            float jumpX = rightMargin - btnSize * 1.35f;
            float jumpY = h - 28 * d - btnSize * 1.35f;
            btnJump.bounds.set(jumpX, jumpY, jumpX + btnSize * 1.35f, jumpY + btnSize * 1.35f);

            // Attack (Mine / Sword) button
            float mineX = jumpX - btnSize * 1.15f;
            float mineY = jumpY - btnSize * 0.45f;
            btnMine.bounds.set(mineX, mineY, mineX + btnSize, mineY + btnSize);

            // Use (Interact / Hand) button
            float useX = jumpX;
            float useY = jumpY - btnSize * 1.15f;
            btnUse.bounds.set(useX, useY, useX + btnSize, useY + btnSize);

            // Inventory button
            float invX = mineX - btnSize * 0.90f;
            float invY = jumpY + btnSize * 0.35f;
            btnInv.bounds.set(invX, invY, invX + btnSize * 0.85f, invY + btnSize * 0.85f);

            btnSneak.bounds.set(0, 0, 0, 0); // Sneak is on btnCenter in styles 0 and 1

        } else if (mControlStyle == 2) {
            // Style 2: Joystick + Action Buttons
            mJoyBaseRadius = 62 * d * scale;
            mJoyKnobRadius = 28 * d * scale;
            mJoyCenterX = 28 * d + mJoyBaseRadius;
            mJoyCenterY = h - 28 * d - mJoyBaseRadius;
            if (!mJoyActive) {
                mJoyKnobX = mJoyCenterX;
                mJoyKnobY = mJoyCenterY;
            }

            // Hide directional D-Pad bounds
            btnUp.bounds.set(0, 0, 0, 0);
            btnDown.bounds.set(0, 0, 0, 0);
            btnLeft.bounds.set(0, 0, 0, 0);
            btnRight.bounds.set(0, 0, 0, 0);
            btnCenter.bounds.set(0, 0, 0, 0);

            // Right action cluster: Jump, Sneak, Sword, Hand, Inventory
            float rightMargin = w - 24 * d;
            float btnSize = 54 * d * scale;

            float jumpX = rightMargin - btnSize;
            float jumpY = h - 30 * d - btnSize * 2.15f;
            btnJump.bounds.set(jumpX, jumpY, jumpX + btnSize, jumpY + btnSize);

            float sneakX = jumpX;
            float sneakY = jumpY + btnSize * 1.20f;
            btnSneak.bounds.set(sneakX, sneakY, sneakX + btnSize, sneakY + btnSize);

            float swordX = jumpX - btnSize * 1.20f;
            float swordY = jumpY;
            btnMine.bounds.set(swordX, swordY, swordX + btnSize, swordY + btnSize);

            float handX = swordX;
            float handY = sneakY;
            btnUse.bounds.set(handX, handY, handX + btnSize, handY + btnSize);

            float invX = swordX - btnSize * 1.05f;
            float invY = handY;
            btnInv.bounds.set(invX, invY, invX + btnSize * 0.85f, invY + btnSize * 0.85f);
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        // CRITICAL: In menus or pause, draw NOTHING. 100% clean native Minecraft look!
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

        if (mControlStyle == 0 || mControlStyle == 1) {
            // Directional controls
            drawButtonBitmap(canvas, btnUp, alpha);
            drawButtonBitmap(canvas, btnDown, alpha);
            drawButtonBitmap(canvas, btnLeft, alpha);
            drawButtonBitmap(canvas, btnRight, alpha);
            drawButtonBitmap(canvas, btnCenter, alpha);

            // Action controls
            drawButtonBitmap(canvas, btnJump, alpha);
            drawButtonBitmap(canvas, btnMine, alpha);
            drawButtonBitmap(canvas, btnUse, alpha);
            drawButtonBitmap(canvas, btnInv, alpha);

        } else if (mControlStyle == 2) {
            // Draw Joystick Base & Knob
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

            // Action controls
            drawButtonBitmap(canvas, btnJump, alpha);
            drawButtonBitmap(canvas, btnSneak, alpha);
            drawButtonBitmap(canvas, btnMine, alpha);
            drawButtonBitmap(canvas, btnUse, alpha);
            drawButtonBitmap(canvas, btnInv, alpha);
        }

        postInvalidateDelayed(16);
    }

    private void drawButtonBitmap(Canvas canvas, VButton btn, int alpha) {
        if (btn.bounds.width() <= 0 || btn.bounds.height() <= 0) return;

        Bitmap b = btn.pressed ? btn.bmpActive : btn.bmpNormal;
        if (b != null) {
            mBitmapPaint.setAlpha(alpha);
            // Apply slight tint highlight when pressed if active bitmap is identical
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
            if (btn.bounds.contains(x, y)) {
                return btn;
            }
        }
        return null;
    }

    private void pressButton(VButton btn, int pointerId) {
        btn.pressed = true;
        btn.pointerId = pointerId;
        if (btn.mouseButton == 1) {
            SDLActivity.onNativeMouse(1, MotionEvent.ACTION_DOWN, 0, 0, false);
        } else if (btn.mouseButton == 2) {
            SDLActivity.onNativeMouse(2, MotionEvent.ACTION_DOWN, 0, 0, false);
        } else if (btn.keyCode != 0) {
            SDLActivity.onNativeKeyDown(btn.keyCode);
        }
    }

    private void releaseButton(VButton btn) {
        btn.pressed = false;
        btn.pointerId = -1;
        if (btn.mouseButton == 1 || btn.mouseButton == 2) {
            SDLActivity.onNativeMouse(0, MotionEvent.ACTION_UP, 0, 0, false);
        } else if (btn.keyCode != 0) {
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
        mCameraPointerId = -1;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        // If not in game, do NOT touch or intercept anything!
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

                // Check Button hits
                VButton hit = findButton(x, y);
                if (hit != null) {
                    pressButton(hit, pointerId);
                    invalidate();
                    return true;
                }

                // Camera Look Touch (Right side of screen outside buttons)
                if (x > getWidth() * 0.35f && mCameraPointerId == -1) {
                    mCameraPointerId = pointerId;
                    mLastCameraX = x;
                    mLastCameraY = y;
                    return true;
                }
                break;
            }

            case MotionEvent.ACTION_MOVE: {
                // Update Joystick
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
                            if (!btn.bounds.contains(curX, curY) && btn.pressed) {
                                releaseButton(btn);
                                invalidate();
                            }
                        }
                    }
                }

                // Update Camera Look
                if (mCameraPointerId != -1) {
                    for (int i = 0; i < event.getPointerCount(); i++) {
                        if (event.getPointerId(i) == mCameraPointerId) {
                            float curX = event.getX(i);
                            float curY = event.getY(i);
                            float dx = curX - mLastCameraX;
                            float dy = curY - mLastCameraY;
                            mLastCameraX = curX;
                            mLastCameraY = curY;
                            // Relative mouse delta for 3D camera
                            SDLActivity.onNativeMouse(0, MotionEvent.ACTION_MOVE, dx, dy, true);
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

                if (pointerId == mCameraPointerId) {
                    mCameraPointerId = -1;
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
