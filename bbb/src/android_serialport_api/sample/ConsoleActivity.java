/*
 * Copyright 2009 Cedric Priscal
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

package android_serialport_api.sample;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.lang.Math;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPut;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONArray;
import org.json.JSONObject;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;
import android_serialport_api.sample.Sending01010101Activity.SendingThread;

public class ConsoleActivity extends SerialPortActivity {

	EditText mReception;
	EditText mReception2;
	String Key = "XAMxMtkBubjscKAyoybOR7m0YlvQI0EQwe1L8AuXSLkkpRP1";
	String Feed = "462938286";
	SendingThread mSendingThread, mSendingThread2;
	private int count =0;
	private int flag_1 = 0;
	private int flag_2 = 0;
	private int flag=0;
	private int flag2=0;
	private int start_xively=0;
	private double difference = 0;
	volatile boolean stop_save = false;
    FileOutputStream fos;
	byte[] mBuffer;
	volatile boolean stop=true;
	ArrayList<byte[]> data_list = new ArrayList<byte[]>();
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.console);

//		setTitle("Loopback test");
		mReception = (EditText) findViewById(R.id.EditTextReception);
		mReception2 = (EditText) findViewById(R.id.EditTextReception2);
		//mReception2 = (EditText) findViewById(R.id.EditTextReception2);

		final Button fileon = (Button)findViewById(R.id.startfile);
        fileon.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				stop=false;
			
			}
		});
        final Button file_logoff = (Button)findViewById(R.id.stopfile);
        file_logoff.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				stop=true;
				stop_save = true;
			}
		});
        final Button xively_on = (Button)findViewById(R.id.xively_on);
        xively_on.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				//this section is only for testing to upload
					
				start_xively=1;
				//flag_1=1;
				
			}
		});
        final Button xively_off = (Button)findViewById(R.id.xively_off);
        xively_off.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				//this section is only for testing to upload
					
				start_xively=0;
		//		mBuffer="t".getBytes();
		//	    mSendingThread = new SendingThread();
		//		mSendingThread.start();
			}
		});
	}
		
	@Override
	protected void onDataReceived(final byte[] buffer, final int size) {
		runOnUiThread(new Runnable() {
			public void run() {
				if (mReception != null) {
					//mReception.append(new String(buffer, 0, size));
				}
				if(stop_save)
				{
					try {
					    File myFile = new File(Environment
					            .getExternalStorageDirectory(), "testing.txt");
					    if (!myFile.exists())
					        myFile.createNewFile();
					    try {
					    	fos = new FileOutputStream(myFile);
					      //  String test="asdfasdfa";
					      //  byte[] data = test.getBytes();
					      //  fos.write(data_list);
//					        for(int i=0;i<data_list.size();i++)
//					       fos.write(data_list.get(i));
					        for (int i = 0; i < data_list.size(); i++)
					            fos.write(data_list.get(i)) ;
					        fos.flush();
					        fos.close();
					    } catch (FileNotFoundException e) {
					        e.printStackTrace();
					    }
					}
					catch (Exception e) {
					    System.out.println("e: " + e);
					}
				}
				String signal = new String(buffer);
//				mReception.append(signal);
//				mBuffer=signal.getBytes();
//			    mSendingThread = new SendingThread();
//				mSendingThread.start();
				if(signal.equals("s"))
				{
				    flag=1;
				    mBuffer=signal.getBytes();
				    mSendingThread = new SendingThread();
					mSendingThread.start();
					mReception.append(signal);
					mReception.append("\n");
					//mReception.setText(mReception.getText() + signal+"  ");
				}
				else
				{	//double num1 = 0,num2 = 0,num3,num4;
					if(signal.charAt(0)=='m')
					{
						flag_1=1;
						flag_2=0;
					String[] str_array = signal.split(":");
					String stringa = str_array[0]; 
					String stringb = str_array[1];
					String[] stringm=stringa.split("m");
					double num1=Double.parseDouble(stringm[1]);
//					if (!stringa.equals(null)) num1 = Double.parseDouble(stringa);
//					if (!stringa.equals(null)) num2 = Double.parseDouble(stringb);
					double num2=Double.parseDouble(stringb);
//					double duration = num1*pow(10,6);
					
					double duration=(num1*10*10*10+num2)/(10*10*10*10*10*10);
					mReception.append(Double.toString(duration));
					mReception.append("\n");
//					if (!stop) data_list.add("m1:".getBytes());
//					if (!stop) data_list.add(Double.toString(num1).getBytes());
//					if (!stop) data_list.add(" ".getBytes());
//					if (!stop) data_list.add("m2:".getBytes());
//					if (!stop) data_list.add(Double.toString(num2).getBytes());
//					if (!stop) data_list.add(" ".getBytes());
//					if (!stop) data_list.add("m_duration:".getBytes());
					if (!stop) 
					{
						data_list.add("mbed2:".getBytes());
						data_list.add(Double.toString(duration).getBytes());
						 data_list.add(" ".getBytes());
					}
					
					difference = duration;
					//	mReception.setText(mReception.getText() + Double.toString(duration)+"  ");
					//mReception.setText(mReception.getText() + signal + " ");
					}
					else
					if(signal.charAt(0)=='n')
					{
						flag_1=0;
						flag_2=1;
					String[] str_array = signal.split(":");
					String stringa = str_array[0]; 
					String stringb = str_array[1];
					String[] stringn=stringa.split("n");
					double num3=Double.parseDouble(stringn[1]);
					double num4=Double.parseDouble(stringb);
					double duration=(num3*10*10*10+num4)/(10*10*10*10*10*10);
					mReception2.append(Double.toString(duration));
					mReception2.append("\n");
					difference = difference - duration;
					if (!stop) 
						{   data_list.add("mbed1:".getBytes());
							data_list.add(Double.toString(duration).getBytes());
						data_list.add(" \n".getBytes());}
					}					
				}
//				flag_1=1;
//				String url="https://api.xively.com/v2/feeds/"+Feed;
//				new HttpAsyncTask().execute(url);
				if(start_xively==1)
				{
				String url="https://api.xively.com/v2/feeds/"+Feed;
				new HttpAsyncTask().execute(url);
				}
			}
			
		});
	}
	protected void onDataReceived2(final byte[] buffer2, final int size2) {
		runOnUiThread(new Runnable() {
			public void run() {
				if (mReception2 != null) {
					mReception2.append(new String(buffer2, 0, size2));
				}
				if(!stop)
				{
					data_list.add(buffer2);
				}
			}
		});
	}
	private class HttpAsyncTask extends AsyncTask<String, Void, String> {
        @Override
        protected String doInBackground(String... urls) {
        	String url=urls[0]; 
        	InputStream inputStream = null;
    		String result = "";
    		try {
    			
    			// 1. create HttpClient
    			HttpClient httpclient = new DefaultHttpClient();
    			
    			// 2. make POST request to the given URL
    			HttpPut httpPut = new HttpPut(url);
    		    
    		    String json = "";
    		    // 3. build jsonObject
    		    String val0=String.valueOf(0);
    		    String val1=String.valueOf(1);
    		    
    		    JSONObject data_point=new JSONObject();
    		   // jsonObject.put("version","1.0.0");
    		    SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    	        String currentTimeStamp = dateFormat.format(new Date());
    		    
    		    if((count%2) == 0){
    		    	data_point.put("value", val0);
    		    }
    		    if((count%2) == 1){
    		    	data_point.put("value", val1);
    		    }
    		    data_point.put("at",currentTimeStamp);
    		    JSONArray datastream=new JSONArray();
    		    datastream.put(data_point);
    		    JSONObject fulldata=new JSONObject();
    		    if(flag_1 == 1){
      		    	fulldata.put("id", "mbed1");
      		    	count++;
      		    }
      		    if(flag_2 == 1){
      		    	fulldata.put("id", "mbed2");
      		    	count++;
      		    }
      		    fulldata.put("datapoints",datastream);
      		    JSONArray data_array= new JSONArray();
      		    data_array.put(fulldata);
    		    JSONObject data_upload=new JSONObject();
    		    data_upload.put("version", "1.0.0");
    			data_upload.put("datastreams", data_array);
    			 
    		    json = data_upload.toString();
    		  
    		    
    		    
    		    
    		
    		    // 4. convert JSONObject to JSON to String
    		   
    		    Log.i("json",json);
    		    
    		    // 5. set json to StringEntity
    		    StringEntity se = new StringEntity(json);
    		    
    		    // 6. set httpPost Entity
    		    httpPut.setEntity(se);
    		    
    		    // 7. Set some headers to inform server about the type of the content   
    		    httpPut.setHeader("X-ApiKey", Key);
    		    httpPut.setHeader("Content-type", "application/json");
    		    //se.setContentType(new BasicHeader(HTTP.CONTENT_TYPE, "application/json"));
    		    
    			// 8. Execute POST request to the given URL
    			HttpResponse httpResponse = httpclient.execute(httpPut);
    			
    			// 9. receive response as inputStream
    			inputStream = httpResponse.getEntity().getContent();
    			
    			// 10. convert inputstream to string
    			if(inputStream != null)
    				result = "work";
    			else
    				result = "Did not work!";
    		
    		} catch (Exception e) {
    			Log.d("InputStream", e.getLocalizedMessage());
    		}
    		
    		// 11. return result
    		return result;
        }
        // onPostExecute displays the results of the AsyncTask.
      
      
    }
	private class SendingThread extends Thread {
		@Override
		public void run() {
			while (!isInterrupted()) {
				try {
					if (mOutputStream != null) {
						if (flag == 1) {
							mOutputStream.write(mBuffer);
							flag = 0;
						}
					} else {
						return;
					}
					if (mOutputStream2 != null) {
						if (flag2 == 1) {
							mOutputStream2.write(mBuffer);
							flag2 = 0;
						}
					} else {
						return;
					}
				} catch (IOException e) {
					e.printStackTrace();
					return;
				}
			}
		}
	}
}
