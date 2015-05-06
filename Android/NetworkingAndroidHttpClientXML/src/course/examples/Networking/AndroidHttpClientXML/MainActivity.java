package course.examples.Networking.AndroidHttpClientXML;

import java.io.IOException;

import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.methods.HttpGet;







//import course.examples.Networking.AndroidHttpClientXML.NetworkingAndroidHttpClientXMLActivity.HttpGetTask;
import android.app.Activity;
import android.net.http.AndroidHttpClient;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

public class MainActivity extends Activity {
	private TextView text;
	//private TextView text2;
	private EditText editText;
	public String URL;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(course.examples.Networking.AndroidHttpClientXML.R.layout.activity_main);

		final Button loadButton = (Button) findViewById(course.examples.Networking.AndroidHttpClientXML.R.id.button1);
		
		text = (TextView) findViewById(course.examples.Networking.AndroidHttpClientXML.R.id.textView1);
		
		editText = (EditText) findViewById(course.examples.Networking.AndroidHttpClientXML.R.id.editText1);
		
		loadButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				
				URL = "http://" + editText.getText() + "/WS_TP/Service1.asmx/ReadPSoC";
				new HttpGetTask().execute();
				
			}
		});
		
	}
	
	private class HttpGetTask extends AsyncTask<Void, Void, String> {
				

		AndroidHttpClient mClient = AndroidHttpClient.newInstance("");

		@Override
		protected String doInBackground(Void... params) {
			HttpGet request = new HttpGet(URL);
			XMLResponseHandler responseHandler = new XMLResponseHandler();
			try {
				return mClient.execute(request, responseHandler);
			} catch (ClientProtocolException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
			return null;
		}

		@Override
		protected void onPostExecute(String result) {
			if (null != mClient)
				mClient.close();
				text.setText(result);
				
		}
	}
}

