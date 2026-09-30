package com.minecraft.console;
import org.libsdl.app.SDLActivity;
import android.os.Bundle;
import android.util.Log;
import android.system.Os;
import android.system.ErrnoException;
import android.content.pm.ActivityInfo;
import android.widget.RelativeLayout;
import com.minecraft.console.R;

public class MainActivity2 extends SDLActivity 
{
	public native int setenv( String env, String value, int over );

    @Override protected void onCreate( Bundle savedInstanceState ) 
    {
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        String directory = getIntent().getStringExtra( "dir" );
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
                try {
                    Class<?> libcore = Class.forName("libcore.io.Libcore");
                    java.lang.reflect.Field osField = libcore.getField("os");
                    Object os = osField.get(null);
                    java.lang.reflect.Method chdir = os.getClass().getMethod("chdir", String.class);
                    chdir.invoke(os, directory);
                    Log.d("ENVTEST", "chdir successful to: " + directory);
                } catch (Throwable t) {
                    Log.e("ENVTEST", "chdir failed", t);
                }
                
                Log.d( "ENVTEST", "MC_PATH=" + Os.getenv("MC_PATH") );
                Log.d( "ENVTEST", "HOME=" + Os.getenv("HOME") );
            }
        }
        catch (ErrnoException e)
        {
            e.printStackTrace();
        }
        super.onCreate( savedInstanceState );

        if (mLayout != null && mSurface != null) {
            VirtualControlsOverlay overlay = new VirtualControlsOverlay(this, mSurface);
            mLayout.addView(overlay, new RelativeLayout.LayoutParams(
                RelativeLayout.LayoutParams.MATCH_PARENT,
                RelativeLayout.LayoutParams.MATCH_PARENT
            ));
        }
    }

    @Override protected String[] getLibraries() 
    {
        return new String[] { "SDL2", "MinecraftClient" };
    }

    @Override protected String getMainFunction() {
        return "main";
    }
}
