package com.example.vehicleapp;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

@SuppressLint("SetTextI18n")
public class MainActivity extends AppCompatActivity {
    Thread Thread1 = null;
    EditText etIP, etPort;
    TextView tvMessages, speedValTV, directionTV, timeTV, distanceTV;
    String SERVER_IP;
    int SERVER_PORT;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        etIP = findViewById(R.id.etIP);
        etPort = findViewById(R.id.etPort);
        tvMessages = findViewById(R.id.tvMessages);
        speedValTV = findViewById(R.id.speedValTV);
        directionTV = findViewById(R.id.directionTV);
        timeTV = findViewById(R.id.timeTV);
        distanceTV = findViewById(R.id.distanceTV);
        Button btnConnect = findViewById(R.id.btnConnect);
        btnConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                tvMessages.setText("");
                SERVER_IP = etIP.getText().toString().trim();
                SERVER_PORT = Integer.parseInt(etPort.getText().toString().trim());
                Thread1 = new Thread(new Thread1());
                Thread1.start();
            }
        });
    }

    private PrintWriter output;
    private BufferedReader input;

    class Thread1 implements Runnable {
        public void run() {
            Socket socket;
            try {
                socket = new Socket(SERVER_IP, SERVER_PORT);
                output = new PrintWriter(socket.getOutputStream());
                input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        tvMessages.setText("Connected\n");
//                        output.write("mobile/update");
//                        output.flush();
                        tvMessages.append("Initiated communication with server\n");
                    }
                });
                new Thread(new Thread2()).start();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        class Thread2 implements Runnable {
            @Override
            public void run() {
                while (true) {
                    try
                    {
                        final String message = input.readLine();
                        if(message != null)
                        {
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    //split message by "/"
                                    String[] messageArray = message.split("/");
                                    String dataType = messageArray[1];
                                    switch (dataType) {
                                        case "speed":
                                            int speed = Integer.parseInt(messageArray[2]);
                                            //set a textview for speed on the screen with the updated value
                                            speedValTV.setText("Speed: " + speed + " mph");
                                            break;
                                        case "steering":
                                            int direction = Integer.parseInt(messageArray[2]);
                                            if (direction == 1) {
                                                //display that the steering direction is right
                                                directionTV.setText("Vehicle is traveling towards the right.");
                                            } else if (direction == 2) {
                                                //display that the steering direction is straight
                                                directionTV.setText("Vehicle is traveling straight.");
                                            } else if (direction == 3) {
                                                //display that the steering direction is left
                                                directionTV.setText("Vehicle is traveling towards the left.");
                                            }
                                            break;
                                        case "time":
                                            int time = Integer.parseInt(messageArray[2]);
                                            if (time > 1000) {
                                                //display that it is currently daytime
                                                timeTV.setText("It is daytime");
                                            } else
                                                //display that it is currently nighttime
                                                timeTV.setText("It is nighttime");
                                            break;
                                        case "distance":
                                            long distance = Long.parseLong(messageArray[2]);
                                            //display current distance in cm
                                            distanceTV.setText("Distance: " + distance + " cm");
                                            break;
                                    }
                                }
                            });
                        }
                        else
                        {
                            Thread1 = new Thread(new Thread1());
                            Thread1.start();
                            return;
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }
}