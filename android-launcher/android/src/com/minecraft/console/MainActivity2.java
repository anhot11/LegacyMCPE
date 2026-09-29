package com.minecraft.console;
import org.libsdl.app.SDLActivity;
import android.os.Bundle;
import android.util.Log;
import android.system.Os;
import android.system.ErrnoException;
import com.minecraft.console.R;

public class MainActivity2 extends SDLActivity 
{
	public native int setenv( String env, String value, int over );

    @Override protected void onCreate( Bundle savedInstanceState ) 
    {
        String directory = getIntent().getStringExtra( "dir" );
        try 
        {
            if (directory.endsWith("/")) 
            {
                directory = directory.substring(0, directory.length() - 1);
            }

            Os.setenv("MC_PATH", directory, true );


            if ( directory.startsWith("/") ) 
            {
                directory = directory.substring(1);
            }

            Os.setenv("HOME", directory, true );
            
            Log.d( "ENVTEST", Os.getenv("MC_PATH") );
            Log.d( "ENVTEST", Os.getenv("HOME") );
        }
        catch (ErrnoException e)
        {
            e.printStackTrace();
        }
        super.onCreate( savedInstanceState );
    }

    @Override protected String[] getLibraries() 
    {
        return new String[] { "SDL2", "MinecraftClient" };
    }

    @Override protected String getMainFunction() {
        return "main";
    }
}
