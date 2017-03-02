package blazers.moduletesting;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI() + getPeople().name);

        // Set call back
        subscribe(new Block() {
            @Override
            public void callBack(String msg) {
                Log.e("====Callback====", msg + "  @ Thread: " + Thread.currentThread().getName());
            }
        });

//        getWindow().getDecorView().postDelayed(new Runnable() {
//            @Override
//            public void run() {
//                stringFromJNI2();
//            }
//        }, 5000);

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native People getPeople();

    public native void subscribe(Block block);

    private static void callBack(String msg) {
        Log.i("====Static====", msg);
    }
}
