/*
 * Copyright 2011 Cedric Priscal
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

import java.io.IOException;
import java.util.Arrays;

import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.content.Intent;
import android.os.Bundle;
//import android.widget.TextView;

public class Sending01010101Activity extends SerialPortActivity {
	private int flag = 0, flag2 = 0;
	private RadioGroup periodGroup;
	private RadioButton periodButton;
	SendingThread mSendingThread, mSendingThread2;
	byte[] mBuffer;
	String period = "1000";
//	Integer period = 1000;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.sending01010101);
//		period=period_text.getText().toString();
		mBuffer = new byte[1024];
	//	mBuffer = period.getBytes();
//		Arrays.fill(mBuffer, period.byteValue());
//		Arrays.fill(mBuffer, (byte) 0x55);
	//	periodGroup = (RadioGroup) findViewById(R.id.periodGroup);
		final Button buttonSetup = (Button)findViewById(R.id.buttonSend500);
	    buttonSetup.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
//				int selectedId = periodGroup.getCheckedRadioButtonId();
//				periodButton = (RadioButton) findViewById(selectedId);
//				if (periodButton.getText().equals("10ms")) period = "10#";
//				if (periodButton.getText().equals("100ms")) period = "100#";
//				if (periodButton.getText().equals("1000ms")) period = "1000#";
//				if (periodButton.getText().equals("10000ms")) period = "10000#";
				period="500#";
				mBuffer=period.getBytes();
				flag = 1;
				if (mSerialPort != null) {
					mSendingThread = new SendingThread();
					mSendingThread.start();
				}
				flag2 = 1;
				if (mSerialPort2 != null) {
					mSendingThread2 = new SendingThread();
					mSendingThread2.start();
				}
			}
		});
	    final Button button1000 = (Button)findViewById(R.id.buttonSend1000);
	    button1000.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
//				int selectedId = periodGroup.getCheckedRadioButtonId();
//				periodButton = (RadioButton) findViewById(selectedId);
//				if (periodButton.getText().equals("10ms")) period = "10#";
//				if (periodButton.getText().equals("100ms")) period = "100#";
//				if (periodButton.getText().equals("1000ms")) period = "1000#";
//				if (periodButton.getText().equals("10000ms")) period = "10000#";
				period="1000#";
				mBuffer=period.getBytes();
				flag = 1;
				if (mSerialPort != null) {
					mSendingThread = new SendingThread();
					mSendingThread.start();
				}
				flag2 = 1;
				if (mSerialPort2 != null) {
					mSendingThread2 = new SendingThread();
					mSendingThread2.start();
				}
			}
		});
	    final Button button2000 = (Button)findViewById(R.id.buttonSend2000);
	    button2000.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
//				int selectedId = periodGroup.getCheckedRadioButtonId();
//				periodButton = (RadioButton) findViewById(selectedId);
//				if (periodButton.getText().equals("10ms")) period = "10#";
//				if (periodButton.getText().equals("100ms")) period = "100#";
//				if (periodButton.getText().equals("1000ms")) period = "1000#";
//				if (periodButton.getText().equals("10000ms")) period = "10000#";
				period="2000#";
				mBuffer=period.getBytes();
				flag = 1;
				if (mSerialPort != null) {
					mSendingThread = new SendingThread();
					mSendingThread.start();
				}
				flag2 = 1;
				if (mSerialPort2 != null) {
					mSendingThread2 = new SendingThread();
					mSendingThread2.start();
				}
			}
		});
	}
	@Override
	protected void onDataReceived(byte[] buffer, int size) {
		// ignore incoming data
	}
	protected void onDataReceived2(byte[] buffer2, int size2) {
		// ignore incoming data
	}
	public class SendingThread extends Thread {
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
