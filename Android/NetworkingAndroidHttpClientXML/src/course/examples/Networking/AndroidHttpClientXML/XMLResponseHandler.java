package course.examples.Networking.AndroidHttpClientXML;

import java.io.IOException;
import java.io.InputStreamReader;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.ResponseHandler;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;

import android.util.Log;

class XMLResponseHandler implements ResponseHandler<String> {

	private static final String TEMP_TAG = "string";

	private String mResult;
	private boolean mIsTemp = true;

	@Override
	public String handleResponse(HttpResponse response)
			throws ClientProtocolException, IOException {
		try {

			// Create the Pull Parser
			XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
			XmlPullParser xpp = factory.newPullParser();

			// Set the Parser's input to be the XML document in the HTTP Response
			xpp.setInput(new InputStreamReader(response.getEntity()
					.getContent()));
			
			// Get the first Parser event and start iterating over the XML document 
			int eventType = xpp.getEventType();

			while (eventType != XmlPullParser.END_DOCUMENT) {

				if (eventType == XmlPullParser.START_TAG) {
					Log.i("XMLResponseHandler", "Start tag");
					
//					if (xpp.getText().equals(TEMP_TAG))
//						mIsTemp = true;
//					else
//						mIsTemp = false;
						
				} else if (eventType == XmlPullParser.END_TAG) {
					Log.i("XMLResponseHandler", "End tag");					
				} else if (eventType == XmlPullParser.TEXT) {
					text(xpp.getText());
				}
				eventType = xpp.next();
			}
			return mResult;
		} catch (XmlPullParserException e) {
		}
		return null;
	}

	

	public void text(String text) {
		if (mIsTemp)
			mResult = text.trim();
		else
			mResult = "Bad XML received";
	}


}