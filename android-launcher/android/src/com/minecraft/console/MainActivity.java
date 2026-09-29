package com.minecraft.console;
import android.app.Activity;
import android.widget.Button;
import android.widget.EditText;
import android.util.Log;
import android.provider.Settings;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.content.Intent;
import android.net.Uri;
import android.content.Context;
import android.content.SharedPreferences;
import android.widget.EditText;
import java.io.File; 
import com.minecraft.console.R;

public class MainActivity extends Activity 
{   
    @Override
    protected void onCreate( Bundle savedInstanceState ) 
    {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_main );

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) 
        {
            if (!Environment.isExternalStorageManager()) 
            {
                try 
                {
                    Intent intent = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
                    intent.addCategory("android.intent.category.DEFAULT");
                    intent.setData(Uri.parse(String.format("package:%s", getPackageName())));
                    startActivity(intent);
                } 
                catch (Exception e) 
                {
                    Intent intent = new Intent();
                    intent.setAction(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION);
                    startActivity(intent);
                }
            }
        }

        SharedPreferences sharedPerf = getSharedPreferences("dirPrefs", Context.MODE_PRIVATE);
        String saveddir = sharedPerf.getString("dir_path", "");

        EditText dir = findViewById( R.id.directory );
        Button b = findViewById( R.id.launch );

        dir.setText( saveddir );

        b.setOnClickListener(v -> 
        {
            Intent intent = new Intent( this, MainActivity2.class );
            SharedPreferences.Editor ed = sharedPerf.edit();
            
            ed.putString("dir_path", dir.getText().toString());
            ed.apply();        
            
            intent.putExtra( "dir", dir.getText().toString() );
            startActivity( intent );
        });
    }
}
