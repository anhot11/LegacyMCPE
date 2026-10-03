package com.minecraft.console;

import org.libsdl.app.SDLActivity;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.system.Os;
import android.system.ErrnoException;
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
import y.MinecraftLegacyP.R;

public class MainActivity2 extends SDLActivity 
{
    private RelativeLayout loadingScreenView;

    @Override protected void onCreate( Bundle savedInstanceState ) 
    {
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setBackgroundDrawable(new android.graphics.drawable.ColorDrawable(android.graphics.Color.BLACK));
        getWindow().setFormat(android.graphics.PixelFormat.RGBA_8888);
        getWindow().setFlags(
            WindowManager.LayoutParams.FLAG_FULLSCREEN | WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS,
            WindowManager.LayoutParams.FLAG_FULLSCREEN | WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS
        );
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            getWindow().getAttributes().layoutInDisplayCutoutMode =
                WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        }

        String directory = getIntent().getStringExtra( "dir" );
        if (directory == null || directory.isEmpty()) {
            directory = getIntent().getStringExtra("game_dir");
        }
        if (directory == null || directory.isEmpty()) {
            android.content.SharedPreferences prefs = getSharedPreferences("dirPrefs", android.content.Context.MODE_PRIVATE);
            directory = prefs.getString("dir_path", null);
        }
        if (directory == null || directory.isEmpty()) {
            java.io.File extFiles = getExternalFilesDir(null);
            if (extFiles != null) {
                directory = extFiles.getAbsolutePath();
            } else {
                directory = getFilesDir().getAbsolutePath();
            }
        }
        try 
        {
            if (directory != null)
            {
                if (directory.endsWith("/")) 
                {
                    directory = directory.substring(0, directory.length() - 1);
                }

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
                
                Log.d( "ENVTEST", "MC_PATH=" + Os.getenv("MC_PATH") );
                Log.d( "ENVTEST", "HOME=" + Os.getenv("HOME") );
            }
        }
        catch (ErrnoException e)
        {
            e.printStackTrace();
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
            VirtualControlsOverlay overlay = new VirtualControlsOverlay(this, mSurface);
            mLayout.addView(overlay, new RelativeLayout.LayoutParams(
                RelativeLayout.LayoutParams.MATCH_PARENT,
                RelativeLayout.LayoutParams.MATCH_PARENT
            ));

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
        title.setText("MCPL-Public");
        title.setTextColor(0xFFFFFFFF);
        title.setTextSize(28);
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
        spinnerParams.setMargins(0, 32, 0, 20);

        TextView subtext = new TextView(this);
        subtext.setText("Cargando juego...");
        subtext.setTextColor(0xFFAAAAAA);
        subtext.setTextSize(15);
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
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            final WindowInsetsController insetsController = getWindow().getInsetsController();
            if (insetsController != null) {
                insetsController.hide(WindowInsets.Type.statusBars() | WindowInsets.Type.navigationBars());
                insetsController.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        } else {
            getWindow().getDecorView().setSystemUiVisibility(
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

    @Override protected String getMainFunction() {
        return "main";
    }
}
