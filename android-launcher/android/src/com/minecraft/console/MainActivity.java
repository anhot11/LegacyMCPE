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
import android.view.Window;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.view.WindowManager;
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
import y.MinecraftLegacyP.R;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class MainActivity extends Activity {
    private static final String TAG = "MCPL-Public";
    private static final int PERMISSION_REQ_CODE = 1001;
    private static final String DATA_URL = "https://github.com/anhot11/LegacyMCPE/releases/download/v1.0.1/MCPL-Data.zip";

    private LinearLayout layoutProgress;
    private LinearLayout layoutDownloadPrompt;
    private LinearLayout layoutDev;
    private ProgressBar progressBar;
    private TextView tvStatus;
    private TextView tvProgress;
    private EditText etDirectory;
    private Button btnLaunch;
    private Button btnDownload;
    private Button btnOpenDev;

    private boolean isWorking = false;
    private boolean devModeActive = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(
            WindowManager.LayoutParams.FLAG_FULLSCREEN | WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS,
            WindowManager.LayoutParams.FLAG_FULLSCREEN | WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS
        );
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            getWindow().getAttributes().layoutInDisplayCutoutMode =
                WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        }
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        hideSystemBars();

        layoutProgress = findViewById(R.id.layout_progress);
        layoutDownloadPrompt = findViewById(R.id.layout_download_prompt);
        layoutDev = findViewById(R.id.layout_dev);
        progressBar = findViewById(R.id.progress_bar);
        tvStatus = findViewById(R.id.tv_status);
        tvProgress = findViewById(R.id.tv_progress);
        etDirectory = findViewById(R.id.directory);
        btnLaunch = findViewById(R.id.launch);
        btnDownload = findViewById(R.id.btn_download);
        btnOpenDev = findViewById(R.id.btn_open_dev);

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

        btnOpenDev.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                devModeActive = true;
                layoutDownloadPrompt.setVisibility(View.GONE);
                layoutProgress.setVisibility(View.GONE);
                layoutDev.setVisibility(View.VISIBLE);
            }
        });

        btnDownload.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String dir = getDefaultGameDir();
                if (!hasStoragePermission(dir)) {
                    requestStoragePermission();
                    return;
                }
                startDownloadAndSetup(dir);
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        hideSystemBars();
        if (!devModeActive && !isWorking) {
            checkAndStart();
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideSystemBars();
        }
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

    private String getDefaultGameDir() {
        // 1. Check if internal storage already has the game installed
        File internalFiles = getFilesDir();
        if (internalFiles != null && isGameInstalled(internalFiles.getAbsolutePath())) {
            return internalFiles.getAbsolutePath();
        }
        // 2. Check if external app files already has the game installed
        File extFiles = getExternalFilesDir(null);
        if (extFiles != null && isGameInstalled(extFiles.getAbsolutePath())) {
            return extFiles.getAbsolutePath();
        }
        // 3. Check if legacy /sdcard/LegacyMCPE is already installed and valid
        File sdcard = Environment.getExternalStorageDirectory();
        if (sdcard != null) {
            File legacy = new File(sdcard, "LegacyMCPE");
            if (isGameInstalled(legacy.getAbsolutePath())) {
                return legacy.getAbsolutePath();
            }
        }
        // 4. Default for fresh download: external app storage (zero permissions needed!)
        if (extFiles != null) {
            return extFiles.getAbsolutePath();
        }
        return getFilesDir().getAbsolutePath();
    }

    private boolean hasStoragePermission(String targetDir) {
        if (targetDir == null || targetDir.isEmpty()) return true;
        File extFiles = getExternalFilesDir(null);
        if (extFiles != null && targetDir.startsWith(extFiles.getAbsolutePath())) {
            return true;
        }
        if (targetDir.startsWith(getFilesDir().getAbsolutePath())) {
            return true;
        }
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

    private boolean isGameInstalled(String dirPath) {
        if (dirPath == null || dirPath.isEmpty()) return false;
        File arcFile = new File(dirPath, "Common/Media/MediaWindows64.arc");
        return arcFile.exists() && arcFile.canRead() && arcFile.length() > 5 * 1024 * 1024;
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

    private void checkAndStart() {
        SharedPreferences prefs = getSharedPreferences("dirPrefs", Context.MODE_PRIVATE);
        String targetDir = prefs.getString("dir_path", getDefaultGameDir());
        if (targetDir == null || targetDir.isEmpty()) {
            targetDir = getDefaultGameDir();
        }

        if (isGameInstalled(targetDir)) {
            // Game is already installed and ready! Launch directly
            launchGame(targetDir);
            return;
        }

        // Check if legacy /sdcard/LegacyMCPE exists and is ready
        File sdcard = Environment.getExternalStorageDirectory();
        if (sdcard != null) {
            File legacy = new File(sdcard, "LegacyMCPE");
            if (isGameInstalled(legacy.getAbsolutePath())) {
                launchGame(legacy.getAbsolutePath());
                return;
            }
        }

        // If an offline APK includes bundled assets in assets/
        if (hasBundledAssets()) {
            startAssetExtraction(targetDir);
            return;
        }

        // Otherwise: show the clean download prompt (prevents double storage!)
        layoutProgress.setVisibility(View.GONE);
        layoutDev.setVisibility(View.GONE);
        layoutDownloadPrompt.setVisibility(View.VISIBLE);
    }

    private void startDownloadAndSetup(final String targetDirPath) {
        isWorking = true;
        layoutDownloadPrompt.setVisibility(View.GONE);
        layoutDev.setVisibility(View.GONE);
        layoutProgress.setVisibility(View.VISIBLE);

        final Handler mainHandler = new Handler(Looper.getMainLooper());

        new Thread(new Runnable() {
            @Override
            public void run() {
                File targetDir = new File(targetDirPath);
                if (!targetDir.exists()) {
                    targetDir.mkdirs();
                }

                File tempZip = new File(targetDir, "MCPL_Download.tmp");

                try {
                    // 1. Download
                    mainHandler.post(new Runnable() {
                        @Override public void run() {
                            tvStatus.setText("Descargando recursos oficiales...");
                            progressBar.setProgress(0);
                            tvProgress.setText("Conectando con el servidor...");
                        }
                    });

                    URL url = new URL(DATA_URL);
                    HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                    connection.setConnectTimeout(15000);
                    connection.setReadTimeout(30000);
                    connection.setInstanceFollowRedirects(true);
                    connection.connect();

                    int responseCode = connection.getResponseCode();
                    if (responseCode == HttpURLConnection.HTTP_MOVED_PERM || responseCode == HttpURLConnection.HTTP_MOVED_TEMP || responseCode == 307 || responseCode == 308) {
                        String newUrl = connection.getHeaderField("Location");
                        connection = (HttpURLConnection) new URL(newUrl).openConnection();
                        connection.connect();
                    }

                    int fileLength = connection.getContentLength();
                    InputStream input = new BufferedInputStream(connection.getInputStream(), 65536);
                    FileOutputStream output = new FileOutputStream(tempZip);

                    byte[] data = new byte[65536];
                    long totalDownloaded = 0;
                    int count;
                    long lastUpdateTime = 0;

                    while ((count = input.read(data)) != -1) {
                        output.write(data, 0, count);
                        totalDownloaded += count;

                        long now = System.currentTimeMillis();
                        if (now - lastUpdateTime > 200) {
                            lastUpdateTime = now;
                            final int percent = fileLength > 0 ? (int) ((totalDownloaded * 100) / fileLength) : 0;
                            final long mb = totalDownloaded / (1024 * 1024);
                            mainHandler.post(new Runnable() {
                                @Override public void run() {
                                    progressBar.setProgress(percent);
                                    tvProgress.setText(percent + "% (" + mb + " MB)");
                                }
                            });
                        }
                    }

                    output.flush();
                    output.close();
                    input.close();

                    // 2. Unpack
                    mainHandler.post(new Runnable() {
                        @Override public void run() {
                            tvStatus.setText("Instalando recursos del juego...");
                            progressBar.setProgress(0);
                            tvProgress.setText("Preparando archivos...");
                        }
                    });

                    unzipFile(tempZip, targetDir, mainHandler);

                    // 3. DELETE TEMPORARY ZIP (Eliminates double space consumption!)
                    if (tempZip.exists()) {
                        tempZip.delete();
                        Log.d(TAG, "Temporary download zip deleted successfully. Storage consumption freed.");
                    }

                    mainHandler.post(new Runnable() {
                        @Override public void run() {
                            progressBar.setProgress(100);
                            tvProgress.setText("100% - ¡Instalación Completada!");
                            tvStatus.setText("Iniciando MCPL-Public...");
                            launchGame(targetDirPath);
                        }
                    });

                } catch (final Exception e) {
                    Log.e(TAG, "Error during download/setup", e);
                    if (tempZip.exists()) {
                        tempZip.delete();
                    }
                    mainHandler.post(new Runnable() {
                        @Override public void run() {
                            isWorking = false;
                            Toast.makeText(MainActivity.this, "Error: " + e.getMessage(), Toast.LENGTH_LONG).show();
                            layoutProgress.setVisibility(View.GONE);
                            layoutDownloadPrompt.setVisibility(View.VISIBLE);
                        }
                    });
                }
            }
        }).start();
    }

    private void unzipFile(File zipFile, File targetDir, final Handler mainHandler) throws IOException {
        InputStream is = new BufferedInputStream(new java.io.FileInputStream(zipFile), 65536);
        ZipInputStream zis = new ZipInputStream(is);
        ZipEntry entry;
        byte[] buffer = new byte[65536];
        long totalBytes = zipFile.length();
        long totalBytesRead = 0;

        while ((entry = zis.getNextEntry()) != null) {
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
                }
                fos.flush();
                fos.close();
            }
            zis.closeEntry();
        }
        zis.close();
    }

    private void startAssetExtraction(final String targetDirPath) {
        isWorking = true;
        layoutDownloadPrompt.setVisibility(View.GONE);
        layoutDev.setVisibility(View.GONE);
        layoutProgress.setVisibility(View.VISIBLE);

        final Handler mainHandler = new Handler(Looper.getMainLooper());

        new Thread(new Runnable() {
            @Override
            public void run() {
                File targetDir = new File(targetDirPath);
                if (!targetDir.exists()) {
                    targetDir.mkdirs();
                }

                byte[] buffer = new byte[65536];
                try {
                    InputStream rawIs = getAssets().open("game_assets.zip");
                    ZipInputStream zis = new ZipInputStream(new BufferedInputStream(rawIs, 65536));
                    ZipEntry entry;

                    while ((entry = zis.getNextEntry()) != null) {
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
                            tvStatus.setText("Iniciando MCPL-Public...");
                            launchGame(targetDirPath);
                        }
                    });

                } catch (final Exception e) {
                    Log.e(TAG, "Error extracting bundled assets", e);
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            isWorking = false;
                            Toast.makeText(MainActivity.this, "Error: " + e.getMessage(), Toast.LENGTH_LONG).show();
                            layoutProgress.setVisibility(View.GONE);
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
        intent.putExtra("game_dir", directory);
        startActivity(intent);
        finish();
    }
}
