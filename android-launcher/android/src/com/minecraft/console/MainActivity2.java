package com.minecraft.console;

import org.libsdl.app.SDLActivity;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.system.Os;
import android.system.ErrnoException;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.graphics.Typeface;
import android.view.Gravity;
import android.view.View;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.view.Window;
import android.view.WindowManager;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;
import java.io.File;
import y.MinecraftLegacyP.R;

public class MainActivity2 extends SDLActivity 
{
    private static final String TAG = "MCPL-MainActivity2";
    private RelativeLayout loadingScreenView;
    private VirtualControlsOverlay mControlsOverlay;

    private boolean isValidGameDir(String path) {
        if (path == null || path.isEmpty()) return false;
        try {
            File arcFile = new File(path, "Common/Media/MediaWindows64.arc");
            return arcFile.exists() && arcFile.length() > 5 * 1024 * 1024;
        } catch (Throwable t) {
            return false;
        }
    }

    private String resolveValidGameDirectory(String preferred) {
        // 1. Check preferred from Intent extras
        if (isValidGameDir(preferred)) {
            Log.i(TAG, "Using preferred directory from Intent: " + preferred);
            return preferred;
        }

        // 2. Check saved SharedPreferences
        try {
            SharedPreferences prefs = getSharedPreferences("dirPrefs", Context.MODE_PRIVATE);
            String saved = prefs.getString("dir_path", null);
            if (isValidGameDir(saved)) {
                Log.i(TAG, "Using saved directory from SharedPreferences: " + saved);
                return saved;
            }
        } catch (Throwable ignored) {}

        // 3. Check legacy /sdcard/LegacyMCPE
        try {
            File sdcard = Environment.getExternalStorageDirectory();
            if (sdcard != null) {
                File legacy = new File(sdcard, "LegacyMCPE");
                if (isValidGameDir(legacy.getAbsolutePath())) {
                    Log.i(TAG, "Found valid game installation in /sdcard/LegacyMCPE");
                    return legacy.getAbsolutePath();
                }
            }
        } catch (Throwable ignored) {}

        // 4. Check app external files dir
        try {
            File extFiles = getExternalFilesDir(null);
            if (extFiles != null && isValidGameDir(extFiles.getAbsolutePath())) {
                Log.i(TAG, "Found valid game installation in externalFilesDir");
                return extFiles.getAbsolutePath();
            }
        } catch (Throwable ignored) {}

        // 5. Check app internal files dir
        try {
            File internal = getFilesDir();
            if (internal != null && isValidGameDir(internal.getAbsolutePath())) {
                Log.i(TAG, "Found valid game installation in internalFilesDir");
                return internal.getAbsolutePath();
            }
        } catch (Throwable ignored) {}

        // Fallbacks if not yet initialized or first boot
        if (preferred != null && !preferred.trim().isEmpty()) {
            return preferred.trim();
        }

        try {
            File legacyFallback = new File(Environment.getExternalStorageDirectory(), "LegacyMCPE");
            if (legacyFallback.exists()) {
                return legacyFallback.getAbsolutePath();
            }
        } catch (Throwable ignored) {}

        try {
            File extFiles = getExternalFilesDir(null);
            if (extFiles != null) return extFiles.getAbsolutePath();
        } catch (Throwable ignored) {}

        try {
            File internal = getFilesDir();
            if (internal != null) return internal.getAbsolutePath();
        } catch (Throwable ignored) {}

        return "/sdcard/LegacyMCPE";
    }

    @Override protected void onCreate( Bundle savedInstanceState ) 
    {
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setBackgroundDrawable(new android.graphics.drawable.ColorDrawable(android.graphics.Color.BLACK));
        getWindow().setFormat(android.graphics.PixelFormat.RGBA_8888);
        getWindow().setFlags(
            WindowManager.LayoutParams.FLAG_FULLSCREEN,
            WindowManager.LayoutParams.FLAG_FULLSCREEN
        );
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            getWindow().getAttributes().layoutInDisplayCutoutMode =
                WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        }

        String rawDir = getIntent().getStringExtra("dir");
        if (rawDir == null || rawDir.isEmpty()) {
            rawDir = getIntent().getStringExtra("game_dir");
        }

        String directory = resolveValidGameDirectory(rawDir);

        try 
        {
            if (directory != null)
            {
                while (directory.endsWith("/") && directory.length() > 1) 
                {
                    directory = directory.substring(0, directory.length() - 1);
                }

                // Persist the resolved path so the native engine and future starts use it
                try {
                    SharedPreferences prefs = getSharedPreferences("dirPrefs", Context.MODE_PRIVATE);
                    prefs.edit().putString("dir_path", directory).apply();
                } catch (Throwable ignored) {}

                Os.setenv("MC_PATH", directory, true );
                Os.setenv("HOME", directory, true );
                Os.setenv("SDL_GAMECONTROLLERCONFIG_FILE", directory + "/gamecontrollerdb.txt", true);
                
                // Set working directory if reflection succeeds (native C code also enforces chdir on startup)
                try {
                    Class<?> libcore = Class.forName("libcore.io.Libcore");
                    java.lang.reflect.Field osField = libcore.getField("os");
                    Object os = osField.get(null);
                    java.lang.reflect.Method chdir = os.getClass().getMethod("chdir", String.class);
                    chdir.invoke(os, directory);
                } catch (Throwable ignored) {}
                
                Log.d( TAG, "MC_PATH=" + Os.getenv("MC_PATH") );
                Log.d( TAG, "HOME=" + Os.getenv("HOME") );
                ensureUiSoundsInstalled(directory);
            }
        }
        catch (ErrnoException e)
        {
            Log.e(TAG, "ErrnoException configuring environment", e);
        }

        super.onCreate( savedInstanceState );
        getWindow().setBackgroundDrawable(new android.graphics.drawable.ColorDrawable(android.graphics.Color.BLACK));
        getWindow().setFormat(android.graphics.PixelFormat.RGBA_8888);
        hideSystemBars();

        if (mLayout != null) {
            mLayout.setBackground(null);
        }

        if (mLayout != null && mSurface != null) {
            // 1. Add virtual touch controls overlay
            final VirtualControlsOverlay overlay = new VirtualControlsOverlay(this, mSurface);
            mControlsOverlay = overlay;
            mLayout.addView(overlay, new RelativeLayout.LayoutParams(
                RelativeLayout.LayoutParams.MATCH_PARENT,
                RelativeLayout.LayoutParams.MATCH_PARENT
            ));

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                getWindow().getDecorView().setOnApplyWindowInsetsListener(new View.OnApplyWindowInsetsListener() {
                    @Override
                    public WindowInsets onApplyWindowInsets(View v, WindowInsets insets) {
                        android.view.DisplayCutout cutout = insets.getDisplayCutout();
                        if (cutout != null) {
                            overlay.setSafeInsets(cutout.getSafeInsetLeft(), cutout.getSafeInsetRight());
                        }
                        return insets;
                    }
                });
            }

            // 2. Add front loading screen in front of controls
            loadingScreenView = createLoadingScreen();
            mLayout.addView(loadingScreenView, new RelativeLayout.LayoutParams(
                RelativeLayout.LayoutParams.MATCH_PARENT,
                RelativeLayout.LayoutParams.MATCH_PARENT
            ));
            loadingScreenView.bringToFront();

            // 3. Fade out loading screen smoothly after 3.5 seconds
            new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
                @Override
                public void run() {
                    if (loadingScreenView != null) {
                        final View toRemove = loadingScreenView;
                        loadingScreenView = null;
                        AlphaAnimation fadeOut = new AlphaAnimation(1.0f, 0.0f);
                        fadeOut.setDuration(400);
                        fadeOut.setAnimationListener(new Animation.AnimationListener() {
                            @Override
                            public void onAnimationStart(Animation animation) {}
                            @Override
                            public void onAnimationEnd(Animation animation) {
                                toRemove.setVisibility(View.GONE);
                                if (mLayout != null) {
                                    mLayout.removeView(toRemove);
                                }
                            }
                            @Override
                            public void onAnimationRepeat(Animation animation) {}
                        });
                        toRemove.startAnimation(fadeOut);
                        new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
                            @Override
                            public void run() {
                                toRemove.setVisibility(View.GONE);
                                if (mLayout != null) {
                                    mLayout.removeView(toRemove);
                                }
                            }
                        }, 500);
                    }
                }
            }, 3500);
        }
    }

    private RelativeLayout createLoadingScreen() {
        RelativeLayout layout = new RelativeLayout(this);
        layout.setBackgroundColor(0xFF111111);
        layout.setClickable(true);
        layout.setFocusable(true);

        LinearLayout centerBox = new LinearLayout(this);
        centerBox.setOrientation(LinearLayout.VERTICAL);
        centerBox.setGravity(Gravity.CENTER);

        RelativeLayout.LayoutParams centerParams = new RelativeLayout.LayoutParams(
            RelativeLayout.LayoutParams.MATCH_PARENT,
            RelativeLayout.LayoutParams.WRAP_CONTENT
        );
        centerParams.addRule(RelativeLayout.CENTER_IN_PARENT);

        TextView title = new TextView(this);
        title.setText("Minecraft: Legacy Console Edition");
        title.setTextColor(0xFFFFFFFF);
        title.setTextSize(24);
        title.setTypeface(null, Typeface.BOLD);
        title.setGravity(Gravity.CENTER);
        title.setSingleLine(true);

        ProgressBar spinner = new ProgressBar(this);
        spinner.setIndeterminate(true);
        LinearLayout.LayoutParams spinnerParams = new LinearLayout.LayoutParams(
            LinearLayout.LayoutParams.WRAP_CONTENT,
            LinearLayout.LayoutParams.WRAP_CONTENT
        );
        spinnerParams.gravity = Gravity.CENTER_HORIZONTAL;
        spinnerParams.setMargins(0, 24, 0, 16);

        TextView subtext = new TextView(this);
        subtext.setText("Iniciando motor gráfico y recursos...");
        subtext.setTextColor(0xFFAAAAAA);
        subtext.setTextSize(14);
        subtext.setGravity(Gravity.CENTER);
        subtext.setSingleLine(true);

        centerBox.addView(title);
        centerBox.addView(spinner, spinnerParams);
        centerBox.addView(subtext);

        layout.addView(centerBox, centerParams);
        return layout;
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideSystemBars();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        hideSystemBars();
    }

    private void hideSystemBars() {
        if (getWindow() == null) return;
        View decorView = getWindow().getDecorView();
        if (decorView == null) return;

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            final WindowInsetsController insetsController = decorView.getWindowInsetsController();
            if (insetsController != null) {
                insetsController.hide(WindowInsets.Type.statusBars() | WindowInsets.Type.navigationBars());
                insetsController.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        } else {
            decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
            );
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            getWindow().getAttributes().layoutInDisplayCutoutMode =
                WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        }
    }

    @Override protected String[] getLibraries() 
    {
        return new String[] { "SDL2", "MinecraftClient" };
    }

    @Override
    public void onBackPressed() {
        // Send ESCAPE key to SDL to close any open container/GUI screen or pause menu
        SDLActivity.onNativeKeyDown(android.view.KeyEvent.KEYCODE_ESCAPE);
        SDLActivity.onNativeKeyUp(android.view.KeyEvent.KEYCODE_ESCAPE);
    }

    private void ensureUiSoundsInstalled(String directory) {
        if (directory == null || directory.isEmpty()) return;
        try {
            File uiSoundDir = new File(directory, "Sound/Minecraft/UI");
            if (!uiSoundDir.exists()) {
                uiSoundDir.mkdirs();
            }
            String[] soundFiles = getAssets().list("sounds/ui");
            if (soundFiles != null) {
                byte[] buf = new byte[4096];
                for (String sf : soundFiles) {
                    File targetSound = new File(uiSoundDir, sf);
                    if (!targetSound.exists() || targetSound.length() == 0) {
                        java.io.InputStream in = getAssets().open("sounds/ui/" + sf);
                        java.io.FileOutputStream out = new java.io.FileOutputStream(targetSound);
                        int len;
                        while ((len = in.read(buf)) > 0) {
                            out.write(buf, 0, len);
                        }
                        out.flush();
                        out.close();
                        in.close();
                        Log.d("MCPL", "Extracted UI sound: " + sf);
                    }
                }
            }
        } catch (Throwable t) {
            Log.e("MCPL", "Failed extracting UI sounds", t);
        }
        try {
            File skinsDir = new File(directory, "skins");
            if (!skinsDir.exists()) skinsDir.mkdirs();
            File mobDir = new File(directory, "Common/res/mob");
            if (!mobDir.exists()) mobDir.mkdirs();
            File mob122Dir = new File(directory, "Common/res/1_2_2/mob");
            if (!mob122Dir.exists()) mob122Dir.mkdirs();

            String[] skinFiles = getAssets().list("skins");
            if (skinFiles != null) {
                byte[] buf = new byte[4096];
                for (String sk : skinFiles) {
                    File target1 = new File(skinsDir, sk);
                    File target2 = new File(mobDir, sk);
                    File target3 = new File(mob122Dir, sk);
                    for (File target : new File[]{target1, target2, target3}) {
                        if (!target.exists() || target.length() == 0) {
                            java.io.InputStream in = getAssets().open("skins/" + sk);
                            java.io.FileOutputStream out = new java.io.FileOutputStream(target);
                            int len;
                            while ((len = in.read(buf)) > 0) {
                                out.write(buf, 0, len);
                            }
                            out.flush();
                            out.close();
                            in.close();
                        }
                    }
                }
            }
        } catch (Throwable t) {
            Log.e("MCPL", "Failed extracting bundled skins", t);
        }
    }

    @Override
    protected boolean onUnhandledMessage(int command, Object param) {
        if (command == 0x8001) {
            boolean inMenu = false;
            if (param instanceof Integer) {
                inMenu = ((Integer) param) == 1;
            }
            if (mControlsOverlay != null) {
                mControlsOverlay.setInMenu(inMenu);
            }
            return true;
        }
        return super.onUnhandledMessage(command, param);
    }

    @Override protected String getMainFunction() {
        return "main";
    }
}
