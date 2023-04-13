package io.github.muntashirakon.zipalign.test;

import android.content.ContentResolver;
import android.database.Cursor;
import android.database.CursorIndexOutOfBoundsException;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.FileUtils;
import android.provider.OpenableColumns;
import android.widget.Toast;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.*;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatButton;
import io.github.muntashirakon.zipalign.ZipAlign;

import java.io.*;

public class MainActivity extends AppCompatActivity {
    private File targetApk;
    private String fileName;
    private final ActivityResultLauncher<String> getSourceUri = registerForActivityResult(
            new ActivityResultContracts.GetContent(),
            uri -> {
                if (uri == null) return;
                fileName = getFileName(getContentResolver(), uri);
                new Thread(() -> {
                    try {
                        File sourceApk = File.createTempFile(fileName, ".apk", getExternalFilesDir(null));
                        try (InputStream is = getContentResolver().openInputStream(uri);
                             FileOutputStream os = new FileOutputStream(sourceApk)) {
                            if (is == null) return;
                            copy(is, os);
                        }
                        zipAlign(sourceApk);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }).start();
            });
    private final ActivityResultLauncher<String> getTargetUri = registerForActivityResult(
            new ActivityResultContracts.CreateDocument(),
            uri -> {
                if (targetApk == null || uri == null) return;
                new Thread(() -> {
                    try (FileInputStream is = new FileInputStream(targetApk);
                         OutputStream os = getContentResolver().openOutputStream(uri)) {
                        if (os == null) return;
                        copy(is, os);
                        displayToast(R.string.apk_successfully_aligned);
                    } catch (IOException e) {
                        e.printStackTrace();
                        displayToast(R.string.could_not_align_apk);
                    } finally {
                        //noinspection ResultOfMethodCallIgnored
                        targetApk.delete();
                    }
                }).start();
            });

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        AppCompatButton selectionBtn = findViewById(R.id.select_apk);
        selectionBtn.setOnClickListener(v -> getSourceUri.launch("application/vnd.android.package-archive"));
    }

    @SuppressWarnings("ResultOfMethodCallIgnored")
    @WorkerThread
    public void zipAlign(@NonNull File sourceApk) throws IOException {
        if (ZipAlign.isZipAligned(sourceApk.getAbsolutePath(), 9, true)) {
            displayToast(R.string.apk_already_aligned);
            return;
        }
        try {
            File targetApk = File.createTempFile(fileName, ".apk", getExternalFilesDir(null));
            boolean success = ZipAlign.doZipAlign(sourceApk.getAbsolutePath(), targetApk.getAbsolutePath(), 9, true);
            if (!success) {
                displayToast(R.string.could_not_align_apk);
                targetApk.delete();
                return;
            }
            this.targetApk = targetApk;
            runOnUiThread(() -> getTargetUri.launch(fileName));
        } finally {
            sourceApk.delete();
        }
    }

    @AnyThread
    public void displayToast(@StringRes int msg) {
        runOnUiThread(() -> Toast.makeText(this, msg, Toast.LENGTH_SHORT).show());
    }

    @WorkerThread
    public static void copy(InputStream inputStream, OutputStream outputStream) throws IOException {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            FileUtils.copy(inputStream, outputStream);
        } else {
            byte[] buffer = new byte[1024 * 4];
            int n;
            while (-1 != (n = inputStream.read(buffer))) {
                outputStream.write(buffer, 0, n);
            }
        }
    }

    @AnyThread
    @Nullable
    public static String getFileName(@NonNull ContentResolver resolver, @NonNull Uri uri) {
        if (uri.getScheme() == null) return null;
        switch (uri.getScheme()) {
            case ContentResolver.SCHEME_CONTENT:
                try (Cursor cursor = resolver.query(uri, null, null, null, null)) {
                    if (cursor == null) return null;
                    int nameIndex = cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME);
                    cursor.moveToFirst();
                    return cursor.getString(nameIndex);
                } catch (CursorIndexOutOfBoundsException ignore) {
                }
            case ContentResolver.SCHEME_FILE:
                if (uri.getPath() == null) return null;
                return new File(uri.getPath()).getName();
            default:
                return null;
        }
    }
}
