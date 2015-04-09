package course.examples.Networking.AndroidHttpClientXML;

import java.io.IOException;

import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.methods.HttpGet;


//import course.examples.Networking.AndroidHttpClientXML.NetworkingAndroidHttpClientXMLActivity.HttpGetTask;
import android.app.Activity;
import android.net.http.AndroidHttpClient;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {
	private TextView text;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(course.examples.Networking.AndroidHttpClientXML.R.layout.activity_main);

		final Button loadButton = (Button) findViewById(course.examples.Networking.AndroidHttpClientXML.R.id.button1);
		
		text = (TextView) findViewById(course.examples.Networking.AndroidHttpClientXML.R.id.textView1);
		
		loadButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				
				new HttpGetTask().execute();
				
			}
		});
	}
	
	private class HttpGetTask extends AsyncTask<Void, Void, String> {

		// IP deve mudar dependendo do dia
		private static final String URL = "http://192.168.0.180/WS_TP/Service1.asmx/ReadPSoC";
				

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

