package com.minecraft.console;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.provider.OpenableColumns;
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
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Locale;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import y.MinecraftLegacyP.R;

public class MainActivity extends Activity {
    private static final String TAG = "MCPL-Launcher";
    private static final int PERMISSION_REQ_CODE = 1001;
    private static final int PICK_ZIP_REQ_CODE = 1002;
    private static final String DATA_URL = "https://github.com/anhot11/LegacyMCPE/releases/download/v1.0.1/MCPL-Data.zip";
    private static final long MIN_FREE_SPACE_BYTES = 500L * 1024 * 1024; // 500 MB

    // Layout containers
    private LinearLayout layoutProgress;
    private LinearLayout layoutDownloadPrompt;
    private LinearLayout layoutLocalDetected;
    private LinearLayout layoutError;
    private LinearLayout layoutDev;

    // Progress widgets
    private ProgressBar progressBar;
    private TextView tvStatus;
    private TextView tvProgress;
    private TextView tvProgressDetails;
    private TextView tvExtractDetail;

    // Prompt widgets
    private TextView tvFreeSpace;
    private TextView tvLocalTitle;
    private Button btnDownload;
    private Button btnInstallLocal;
    private Button btnPickZip;
    private Button btnOpenDev;

    // Error widgets
    private TextView tvErrorMsg;
    private Button btnRetry;
    private Button btnErrorBack;

    // Dev widgets
    private EditText etDirectory;
    private Button btnLaunch;
    private Button btnDevReset;
    private Button btnDevBack;

    private boolean isWorking = false;
    private boolean devModeActive = false;
    private File detectedLocalZip = null;
    private Runnable lastFailedAction = null;

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

        initViews();
        setupListeners();
    }

    private void initViews() {
        layoutProgress = findViewById(R.id.layout_progress);
        layoutDownloadPrompt = findViewById(R.id.layout_download_prompt);
        layoutLocalDetected = findViewById(R.id.layout_local_detected);
        layoutError = findViewById(R.id.layout_error);
        layoutDev = findViewById(R.id.layout_dev);

        progressBar = findViewById(R.id.progress_bar);
        tvStatus = findViewById(R.id.tv_status);
        tvProgress = findViewById(R.id.tv_progress);
        tvProgressDetails = findViewById(R.id.tv_progress_details);
        tvExtractDetail = findViewById(R.id.tv_extract_detail);

        tvFreeSpace = findViewById(R.id.tv_free_space);
        tvLocalTitle = findViewById(R.id.tv_local_title);
        btnDownload = findViewById(R.id.btn_download);
        btnInstallLocal = findViewById(R.id.btn_install_local);
        btnPickZip = findViewById(R.id.btn_pick_zip);
        btnOpenDev = findViewById(R.id.btn_open_dev);

        tvErrorMsg = findViewById(R.id.tv_error_msg);
        btnRetry = findViewById(R.id.btn_retry);
        btnErrorBack = findViewById(R.id.btn_error_back);

        etDirectory = findViewById(R.id.directory);
        btnLaunch = findViewById(R.id.launch);
        btnDevReset = findViewById(R.id.btn_dev_reset);
        btnDevBack = findViewById(R.id.btn_dev_back);

        SharedPreferences prefs = getSharedPreferences("dirPrefs", Context.MODE_PRIVATE);
        String savedDir = prefs.getString("dir_path", getDefaultGameDir());
        etDirectory.setText(savedDir);
    }

    private void setupListeners() {
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

        btnDevReset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String defaultPath = getDefaultGameDir();
                etDirectory.setText(defaultPath);
                Toast.makeText(MainActivity.this, "Ruta restablecida a la predeterminada", Toast.LENGTH_SHORT).show();
            }
        });

        btnDevBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                devModeActive = false;
                layoutDev.setVisibility(View.GONE);
                checkAndStart();
            }
        });

        btnOpenDev.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                devModeActive = true;
                layoutDownloadPrompt.setVisibility(View.GONE);
                layoutProgress.setVisibility(View.GONE);
                layoutError.setVisibility(View.GONE);
                layoutDev.setVisibility(View.VISIBLE);
            }
        });

        btnDownload.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final String dir = getDefaultGameDir();
                if (!hasStoragePermission(dir)) {
                    requestStoragePermission();
                    return;
                }
                lastFailedAction = new Runnable() {
                    @Override
                    public void run() {
                        startDownloadAndSetup(dir);
                    }
                };
                startDownloadAndSetup(dir);
            }
        });

        btnInstallLocal.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (detectedLocalZip != null && detectedLocalZip.exists()) {
                    final String dir = getDefaultGameDir();
                    lastFailedAction = new Runnable() {
                        @Override
                        public void run() {
                            startLocalFileExtraction(detectedLocalZip, dir);
                        }
                    };
                    startLocalFileExtraction(detectedLocalZip, dir);
                }
            }
        });

        btnPickZip.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
                    intent.setType("application/zip");
                    intent.addCategory(Intent.CATEGORY_OPENABLE);
                    startActivityForResult(Intent.createChooser(intent, "Selecciona MCPL-Data.zip"), PICK_ZIP_REQ_CODE);
                } catch (Exception e) {
                    Toast.makeText(MainActivity.this, "No se pudo abrir el explorador de archivos", Toast.LENGTH_SHORT).show();
                }
            }
        });

        btnRetry.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                layoutError.setVisibility(View.GONE);
                if (lastFailedAction != null) {
                    lastFailedAction.run();
                } else {
                    checkAndStart();
                }
            }
        });

        btnErrorBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                isWorking = false;
                layoutError.setVisibility(View.GONE);
                checkAndStart();
            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == PICK_ZIP_REQ_CODE && resultCode == RESULT_OK && data != null) {
            Uri uri = data.getData();
            if (uri != null) {
                final String dir = getDefaultGameDir();
                startUriExtraction(uri, dir);
            }
        }
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
        // 4. Default for fresh download: external app storage (zero permissions needed on Android 11+!)
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

    private File findLocalDataZip() {
        File[] searchPaths = new File[]{
            new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS), "MCPL-Data.zip"),
            new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS), "LegacyMCPE/MCPL-Data.zip"),
            new File(Environment.getExternalStorageDirectory(), "MCPL-Data.zip"),
            new File(Environment.getExternalStorageDirectory(), "LegacyMCPE/MCPL-Data.zip"),
            new File(Environment.getExternalStorageDirectory(), "Download/MCPL-Data.zip")
        };

        for (File candidate : searchPaths) {
            if (candidate != null && candidate.exists() && candidate.isFile() && candidate.length() > 10 * 1024 * 1024) {
                return candidate;
            }
        }
        return null;
    }

    private void checkAndStart() {
        SharedPreferences prefs = getSharedPreferences("dirPrefs", Context.MODE_PRIVATE);
        String targetDir = prefs.getString("dir_path", getDefaultGameDir());
        if (targetDir == null || targetDir.isEmpty()) {
            targetDir = getDefaultGameDir();
        }

        if (isGameInstalled(targetDir)) {
            // Game is already installed and ready! Launch directly
            showQuickLaunch(targetDir);
            return;
        }

        // Check if legacy /sdcard/LegacyMCPE exists and is ready
        File sdcard = Environment.getExternalStorageDirectory();
        if (sdcard != null) {
            File legacy = new File(sdcard, "LegacyMCPE");
            if (isGameInstalled(legacy.getAbsolutePath())) {
                showQuickLaunch(legacy.getAbsolutePath());
                return;
            }
        }

        // If an offline APK includes bundled assets in assets/
        if (hasBundledAssets()) {
            startBundledAssetExtraction(targetDir);
            return;
        }

        // Check free space
        File targetDirFile = new File(targetDir);
        long freeBytes = targetDirFile.getFreeSpace();
        long freeMb = freeBytes / (1024 * 1024);
        if (freeMb > 0) {
            tvFreeSpace.setText("Tamaño del paquete: ~238 MB (" + freeMb + " MB libres en almacenamiento)");
        } else {
            tvFreeSpace.setText("Tamaño del paquete: ~238 MB");
        }

        // Check if a local MCPL-Data.zip exists in Downloads
        detectedLocalZip = findLocalDataZip();
        if (detectedLocalZip != null) {
            long sizeMb = detectedLocalZip.length() / (1024 * 1024);
            tvLocalTitle.setText("📦 Se detectó " + detectedLocalZip.getName() + " (" + sizeMb + " MB) en almacenamiento");
            layoutLocalDetected.setVisibility(View.VISIBLE);
        } else {
            layoutLocalDetected.setVisibility(View.GONE);
        }

        layoutProgress.setVisibility(View.GONE);
        layoutError.setVisibility(View.GONE);
        layoutDev.setVisibility(View.GONE);
        layoutDownloadPrompt.setVisibility(View.VISIBLE);
    }

    private void showQuickLaunch(final String targetDirPath) {
        layoutDownloadPrompt.setVisibility(View.GONE);
        layoutError.setVisibility(View.GONE);
        layoutDev.setVisibility(View.GONE);
        layoutProgress.setVisibility(View.VISIBLE);

        tvStatus.setText("Iniciando MCPL-Public...");
        progressBar.setProgress(100);
        tvProgress.setText("100%");
        tvProgressDetails.setText("Cargando motor del juego...");
        tvExtractDetail.setVisibility(View.GONE);

        new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
            @Override
            public void run() {
                launchGame(targetDirPath);
            }
        }, 400);
    }

    private void showError(String message) {
        isWorking = false;
        layoutProgress.setVisibility(View.GONE);
        layoutDownloadPrompt.setVisibility(View.GONE);
        layoutDev.setVisibility(View.GONE);
        tvErrorMsg.setText(message);
        layoutError.setVisibility(View.VISIBLE);
    }

    private void startDownloadAndSetup(final String targetDirPath) {
        isWorking = true;
        layoutDownloadPrompt.setVisibility(View.GONE);
        layoutError.setVisibility(View.GONE);
        layoutDev.setVisibility(View.GONE);
        layoutProgress.setVisibility(View.VISIBLE);
        tvExtractDetail.setVisibility(View.GONE);

        final Handler mainHandler = new Handler(Looper.getMainLooper());

        new Thread(new Runnable() {
            @Override
            public void run() {
                File targetDir = new File(targetDirPath);
                if (!targetDir.exists()) {
                    targetDir.mkdirs();
                }

                // Check free space before downloading
                long freeSpace = targetDir.getFreeSpace();
                if (freeSpace > 0 && freeSpace < MIN_FREE_SPACE_BYTES) {
                    final long freeMb = freeSpace / (1024 * 1024);
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            showError("Espacio insuficiente en disco (" + freeMb + " MB libres). Se requieren al menos 500 MB libres para instalar.");
                        }
                    });
                    return;
                }

                File tempZip = new File(targetDir, "MCPL_Download.tmp");

                try {
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            tvStatus.setText("Descargando recursos oficiales...");
                            progressBar.setProgress(0);
                            tvProgress.setText("0%");
                            tvProgressDetails.setText("Conectando con el servidor...");
                        }
                    });

                    // Follow redirects (GitHub Releases redirect to AWS S3)
                    String currentUrl = DATA_URL;
                    HttpURLConnection connection = null;
                    int redirectCount = 0;

                    while (redirectCount++ < 10) {
                        URL url = new URL(currentUrl);
                        connection = (HttpURLConnection) url.openConnection();
                        connection.setRequestProperty("User-Agent", "Mozilla/5.0 MCPL-Launcher/1.0 (Android)");
                        connection.setRequestProperty("Accept-Encoding", "identity");
                        connection.setConnectTimeout(20000);
                        connection.setReadTimeout(45000);
                        connection.setInstanceFollowRedirects(false);
                        connection.connect();

                        int responseCode = connection.getResponseCode();
                        if (responseCode == HttpURLConnection.HTTP_MOVED_PERM ||
                            responseCode == HttpURLConnection.HTTP_MOVED_TEMP ||
                            responseCode == HttpURLConnection.HTTP_SEE_OTHER ||
                            responseCode == 307 || responseCode == 308) {
                            String newUrl = connection.getHeaderField("Location");
                            connection.disconnect();
                            if (newUrl != null && !newUrl.isEmpty()) {
                                currentUrl = newUrl;
                                continue;
                            }
                        }
                        break;
                    }

                    if (connection == null || connection.getResponseCode() != HttpURLConnection.HTTP_OK) {
                        int code = connection != null ? connection.getResponseCode() : -1;
                        throw new IOException("Error HTTP " + code + " al descargar recursos desde GitHub");
                    }

                    int fileLength = connection.getContentLength();
                    InputStream input = new BufferedInputStream(connection.getInputStream(), 65536);
                    FileOutputStream output = new FileOutputStream(tempZip);

                    byte[] data = new byte[65536];
                    long totalDownloaded = 0;
                    int count;
                    long lastUpdateTime = System.currentTimeMillis();
                    long lastBytes = 0;

                    while ((count = input.read(data)) != -1) {
                        output.write(data, 0, count);
                        totalDownloaded += count;

                        long now = System.currentTimeMillis();
                        if (now - lastUpdateTime >= 300) {
                            long elapsed = now - lastUpdateTime;
                            long bytesDelta = totalDownloaded - lastBytes;
                            float speedMbSec = (float) bytesDelta / (elapsed / 1000.0f) / (1024.0f * 1024.0f);
                            final int percent = fileLength > 0 ? (int) ((totalDownloaded * 100) / fileLength) : 0;
                            final long downloadedMb = totalDownloaded / (1024 * 1024);
                            final long totalMb = fileLength > 0 ? fileLength / (1024 * 1024) : 238;

                            String etaStr = "";
                            if (speedMbSec > 0.05f && fileLength > totalDownloaded) {
                                long remainingSec = (long) ((fileLength - totalDownloaded) / (speedMbSec * 1024 * 1024));
                                if (remainingSec < 60) {
                                    etaStr = " • ETA: " + remainingSec + "s";
                                } else {
                                    etaStr = " • ETA: " + (remainingSec / 60) + "m " + (remainingSec % 60) + "s";
                                }
                            }

                            final String details = String.format(Locale.US, "%d MB / %d MB (%.1f MB/s)%s",
                                    downloadedMb, totalMb, speedMbSec, etaStr);

                            mainHandler.post(new Runnable() {
                                @Override
                                public void run() {
                                    progressBar.setProgress(percent);
                                    tvProgress.setText(percent + "%");
                                    tvProgressDetails.setText(details);
                                }
                            });

                            lastUpdateTime = now;
                            lastBytes = totalDownloaded;
                        }
                    }

                    output.flush();
                    output.close();
                    input.close();
                    connection.disconnect();

                    // 2. Unpack
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            tvStatus.setText("Instalando recursos del juego...");
                            progressBar.setProgress(0);
                            tvProgress.setText("0%");
                            tvProgressDetails.setText("Iniciando extracción...");
                            tvExtractDetail.setVisibility(View.VISIBLE);
                        }
                    });

                    unzipFile(tempZip, targetDir, mainHandler);

                    // 3. Delete temporary zip file
                    if (tempZip.exists()) {
                        tempZip.delete();
                        Log.d(TAG, "Temporary download zip deleted.");
                    }

                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            progressBar.setProgress(100);
                            tvProgress.setText("100%");
                            tvStatus.setText("¡Instalación completada!");
                            tvProgressDetails.setText("Iniciando juego...");
                            tvExtractDetail.setVisibility(View.GONE);
                            launchGame(targetDirPath);
                        }
                    });

                } catch (final Exception e) {
                    Log.e(TAG, "Error during download/setup", e);
                    if (tempZip.exists()) {
                        tempZip.delete();
                    }
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            showError("Error durante la descarga: " + e.getMessage());
                        }
                    });
                }
            }
        }).start();
    }

    private void startLocalFileExtraction(final File localZip, final String targetDirPath) {
        isWorking = true;
        layoutDownloadPrompt.setVisibility(View.GONE);
        layoutError.setVisibility(View.GONE);
        layoutDev.setVisibility(View.GONE);
        layoutProgress.setVisibility(View.VISIBLE);
        tvExtractDetail.setVisibility(View.VISIBLE);

        final Handler mainHandler = new Handler(Looper.getMainLooper());

        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    File targetDir = new File(targetDirPath);
                    if (!targetDir.exists()) {
                        targetDir.mkdirs();
                    }

                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            tvStatus.setText("Extrayendo archivo local...");
                            progressBar.setProgress(0);
                            tvProgress.setText("0%");
                            tvProgressDetails.setText("Descomprimiendo " + localZip.getName() + "...");
                        }
                    });

                    unzipFile(localZip, targetDir, mainHandler);

                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            progressBar.setProgress(100);
                            tvProgress.setText("100%");
                            tvStatus.setText("¡Instalación completada!");
                            tvProgressDetails.setText("Iniciando juego...");
                            tvExtractDetail.setVisibility(View.GONE);
                            launchGame(targetDirPath);
                        }
                    });

                } catch (final Exception e) {
                    Log.e(TAG, "Error extracting local zip", e);
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            showError("Error al extraer archivo local: " + e.getMessage());
                        }
                    });
                }
            }
        }).start();
    }

    private void startUriExtraction(final Uri uri, final String targetDirPath) {
        isWorking = true;
        layoutDownloadPrompt.setVisibility(View.GONE);
        layoutError.setVisibility(View.GONE);
        layoutDev.setVisibility(View.GONE);
        layoutProgress.setVisibility(View.VISIBLE);
        tvExtractDetail.setVisibility(View.VISIBLE);

        final Handler mainHandler = new Handler(Looper.getMainLooper());

        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    File targetDir = new File(targetDirPath);
                    if (!targetDir.exists()) {
                        targetDir.mkdirs();
                    }

                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            tvStatus.setText("Instalando desde archivo seleccionado...");
                            progressBar.setProgress(0);
                            tvProgress.setText("0%");
                            tvProgressDetails.setText("Preparando extracción...");
                        }
                    });

                    InputStream rawIs = getContentResolver().openInputStream(uri);
                    if (rawIs == null) {
                        throw new IOException("No se pudo abrir el archivo seleccionado.");
                    }
                    unzipStream(rawIs, targetDir, mainHandler, 0);

                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            progressBar.setProgress(100);
                            tvProgress.setText("100%");
                            tvStatus.setText("¡Instalación completada!");
                            tvProgressDetails.setText("Iniciando juego...");
                            tvExtractDetail.setVisibility(View.GONE);
                            launchGame(targetDirPath);
                        }
                    });

                } catch (final Exception e) {
                    Log.e(TAG, "Error extracting selected uri", e);
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            showError("Error al instalar desde archivo: " + e.getMessage());
                        }
                    });
                }
            }
        }).start();
    }

    private void startBundledAssetExtraction(final String targetDirPath) {
        isWorking = true;
        layoutDownloadPrompt.setVisibility(View.GONE);
        layoutError.setVisibility(View.GONE);
        layoutDev.setVisibility(View.GONE);
        layoutProgress.setVisibility(View.VISIBLE);
        tvExtractDetail.setVisibility(View.VISIBLE);

        final Handler mainHandler = new Handler(Looper.getMainLooper());

        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    File targetDir = new File(targetDirPath);
                    if (!targetDir.exists()) {
                        targetDir.mkdirs();
                    }

                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            tvStatus.setText("Instalando recursos integrados...");
                            progressBar.setProgress(0);
                            tvProgress.setText("0%");
                            tvProgressDetails.setText("Descomprimiendo...");
                        }
                    });

                    InputStream rawIs = getAssets().open("game_assets.zip");
                    unzipStream(rawIs, targetDir, mainHandler, 0);

                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            progressBar.setProgress(100);
                            tvProgress.setText("100%");
                            tvStatus.setText("¡Instalación completada!");
                            tvProgressDetails.setText("Iniciando juego...");
                            tvExtractDetail.setVisibility(View.GONE);
                            launchGame(targetDirPath);
                        }
                    });

                } catch (final Exception e) {
                    Log.e(TAG, "Error extracting bundled assets", e);
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            showError("Error al extraer recursos integrados: " + e.getMessage());
                        }
                    });
                }
            }
        }).start();
    }

    private void unzipFile(File zipFile, File targetDir, Handler mainHandler) throws IOException {
        long zipSize = zipFile.length();
        InputStream is = new BufferedInputStream(new java.io.FileInputStream(zipFile), 65536);
        unzipStream(is, targetDir, mainHandler, zipSize);
    }

    private void unzipStream(InputStream inputStream, File targetDir, final Handler mainHandler, long approxZipSize) throws IOException {
        ZipInputStream zis = new ZipInputStream(new BufferedInputStream(inputStream, 65536));
        ZipEntry entry;
        byte[] buffer = new byte[65536];
        int fileCount = 0;
        long lastUiUpdate = System.currentTimeMillis();

        while ((entry = zis.getNextEntry()) != null) {
            String entryName = entry.getName();
            File outFile = new File(targetDir, entryName);
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
                fileCount++;

                long now = System.currentTimeMillis();
                if (now - lastUiUpdate >= 120) {
                    lastUiUpdate = now;
                    final int count = fileCount;
                    final String currentFile = entryName;
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            // Progress advances smoothly up to 99% as files are extracted
                            int progress = Math.min(99, count / 15);
                            progressBar.setProgress(progress);
                            tvProgress.setText(progress + "%");
                            tvProgressDetails.setText("Archivos instalados: " + count);
                            tvExtractDetail.setText(currentFile);
                        }
                    });
                }
            }
            zis.closeEntry();
        }
        zis.close();
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
                        InputStream in = getAssets().open("sounds/ui/" + sf);
                        FileOutputStream out = new FileOutputStream(targetSound);
                        int len;
                        while ((len = in.read(buf)) > 0) {
                            out.write(buf, 0, len);
                        }
                        out.flush();
                        out.close();
                        in.close();
                        Log.d(TAG, "Extracted UI sound: " + sf);
                    }
                }
            }
        } catch (Throwable t) {
            Log.e(TAG, "Failed extracting UI sounds", t);
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
                            InputStream in = getAssets().open("skins/" + sk);
                            FileOutputStream out = new FileOutputStream(target);
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
            Log.e(TAG, "Failed extracting bundled skins", t);
        }
    }

    private void launchGame(String directory) {
        ensureUiSoundsInstalled(directory);
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
