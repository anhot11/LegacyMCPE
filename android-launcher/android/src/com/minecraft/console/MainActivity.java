package com.minecraft.console;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetFileDescriptor;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.provider.Settings;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class MainActivity extends Activity {
    private static final String TAG = "LegacyMCPE";
    private static final int PERMISSION_REQ_CODE = 1001;

    private LinearLayout layoutExtract;
    private LinearLayout layoutDev;
    private ProgressBar progressBar;
    private TextView tvStatus;
    private TextView tvProgress;
    private EditText etDirectory;
    private Button btnLaunch;
    private Button btnDevToggle;

    private boolean isExtracting = false;
    private boolean devModeActive = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        layoutExtract = findViewById(R.id.layout_extract);
        layoutDev = findViewById(R.id.layout_dev);
        progressBar = findViewById(R.id.extract_progress);
        tvStatus = findViewById(R.id.tv_status);
        tvProgress = findViewById(R.id.tv_progress);
        etDirectory = findViewById(R.id.directory);
        btnLaunch = findViewById(R.id.launch);
        btnDevToggle = findViewById(R.id.btn_dev_toggle);

        SharedPreferences prefs = getSharedPreferences("dirPrefs", Context.MODE_PRIVATE);
        String defaultPath = getDefaultGameDir();
        String savedDir = prefs.getString("dir_path", defaultPath);
        etDirectory.setText(savedDir);

        btnLaunch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String path = etDirectory.getText().toString().trim();
                if (path.isEmpty()) {
                    path = getDefaultGameDir();
                }
                launchGame(path);
            }
        });

        btnDevToggle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                devModeActive = true;
                layoutExtract.setVisibility(View.GONE);
                layoutDev.setVisibility(View.VISIBLE);
                btnDevToggle.setVisibility(View.GONE);
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (!devModeActive && !isExtracting) {
            checkAndStart();
        }
    }

    private String getDefaultGameDir() {
        File sdcard = Environment.getExternalStorageDirectory();
        if (sdcard != null) {
            return new File(sdcard, "LegacyMCPE").getAbsolutePath();
        }
        return "/sdcard/LegacyMCPE";
    }

    private boolean hasStoragePermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            return Environment.isExternalStorageManager();
        } else {
            return checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED;
        }
    }

    private void requestStoragePermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            try {
                Intent intent = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
                intent.addCategory("android.intent.category.DEFAULT");
                intent.setData(Uri.parse(String.format("package:%s", getPackageName())));
                startActivity(intent);
            } catch (Exception e) {
                Intent intent = new Intent();
                intent.setAction(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION);
                startActivity(intent);
            }
        } else {
            requestPermissions(new String[]{
                    Manifest.permission.READ_EXTERNAL_STORAGE,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE
            }, PERMISSION_REQ_CODE);
        }
    }

    private boolean hasBundledAssets() {
        try {
            InputStream is = getAssets().open("game_assets.zip");
            is.close();
            return true;
        } catch (IOException e) {
            return false;
        }
    }

    private boolean isGameInstalled(String dirPath) {
        File arcFile = new File(dirPath, "Common/Media/MediaWindows64.arc");
        return arcFile.exists() && arcFile.length() > 5 * 1024 * 1024;
    }

    private void checkAndStart() {
        if (!hasStoragePermission()) {
            requestStoragePermission();
            return;
        }

        SharedPreferences prefs = getSharedPreferences("dirPrefs", Context.MODE_PRIVATE);
        String targetDir = prefs.getString("dir_path", getDefaultGameDir());
        if (targetDir == null || targetDir.isEmpty()) {
            targetDir = getDefaultGameDir();
        }

        if (isGameInstalled(targetDir)) {
            // Already installed! Launch immediately
            launchGame(targetDir);
            return;
        }

        // Check if bundled assets are available inside APK
        if (hasBundledAssets()) {
            startAssetExtraction(targetDir);
        } else {
            // Dev mode: Show directory selector
            layoutExtract.setVisibility(View.GONE);
            layoutDev.setVisibility(View.VISIBLE);
        }
    }

    private void startAssetExtraction(final String targetDirPath) {
        isExtracting = true;
        layoutExtract.setVisibility(View.VISIBLE);
        layoutDev.setVisibility(View.GONE);
        btnDevToggle.setVisibility(View.VISIBLE);

        final Handler mainHandler = new Handler(Looper.getMainLooper());

        new Thread(new Runnable() {
            @Override
            public void run() {
                File targetDir = new File(targetDirPath);
                if (!targetDir.exists()) {
                    targetDir.mkdirs();
                }

                long totalBytes = 230 * 1024 * 1024L; // Default estimate
                try {
                    AssetFileDescriptor afd = getAssets().openFd("game_assets.zip");
                    if (afd != null && afd.getLength() > 0) {
                        totalBytes = afd.getLength();
                    }
                    if (afd != null) {
                        afd.close();
                    }
                } catch (Exception ignored) {}

                byte[] buffer = new byte[65536];
                long totalBytesRead = 0;

                try {
                    InputStream rawIs = getAssets().open("game_assets.zip");
                    ZipInputStream zis = new ZipInputStream(new BufferedInputStream(rawIs, 65536));
                    ZipEntry entry;

                    while ((entry = zis.getNextEntry()) != null) {
                        if (devModeActive) {
                            zis.close();
                            return;
                        }

                        File outFile = new File(targetDir, entry.getName());
                        if (entry.isDirectory()) {
                            outFile.mkdirs();
                        } else {
                            File parent = outFile.getParentFile();
                            if (parent != null && !parent.exists()) {
                                parent.mkdirs();
                            }

                            FileOutputStream fos = new FileOutputStream(outFile);
                            int len;
                            while ((len = zis.read(buffer)) > 0) {
                                fos.write(buffer, 0, len);
                                totalBytesRead += len;

                                final int percent = (int) Math.min(99, (totalBytesRead * 100) / totalBytes);
                                final long mbRead = totalBytesRead / (1024 * 1024);

                                mainHandler.post(new Runnable() {
                                    @Override
                                    public void run() {
                                        progressBar.setProgress(percent);
                                        tvProgress.setText(percent + "% (" + mbRead + " MB)");
                                    }
                                });
                            }
                            fos.flush();
                            fos.close();
                        }
                        zis.closeEntry();
                    }
                    zis.close();

                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            progressBar.setProgress(100);
                            tvProgress.setText("100% - ¡Completado!");
                            tvStatus.setText("¡Iniciando Legacy MCPE!");
                            launchGame(targetDirPath);
                        }
                    });

                } catch (final Exception e) {
                    Log.e(TAG, "Error extracting game assets", e);
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            isExtracting = false;
                            Toast.makeText(MainActivity.this, "Error al extraer recursos: " + e.getMessage(), Toast.LENGTH_LONG).show();
                            layoutExtract.setVisibility(View.GONE);
                            layoutDev.setVisibility(View.VISIBLE);
                        }
                    });
                }
            }
        }).start();
    }

    private void launchGame(String directory) {
        SharedPreferences prefs = getSharedPreferences("dirPrefs", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();
        editor.putString("dir_path", directory);
        editor.apply();

        Intent intent = new Intent(MainActivity.this, MainActivity2.class);
        intent.putExtra("dir", directory);
        startActivity(intent);
        finish();
    }
}
