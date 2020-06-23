package com.example.vehicleapp;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.StrictMode;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

@SuppressLint("SetTextI18n")
public class MainActivity extends AppCompatActivity {
    EditText etIP, etPort;
    TextView tvMessages, speedValTV, directionTV, timeTV, distanceTV;
    String SERVER_IP;
    int SERVER_PORT;
    DataInputStream din;
    DataOutputStream dout;
    Socket socket;
    final String IPV4_REGEX =
            "^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\." +
            "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\." +
            "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\." +
            "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$";
    final Pattern IPV4_ADDR_PATTERN = Pattern.compile(IPV4_REGEX);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);

        setContentView(R.layout.activity_main);

        etIP = findViewById(R.id.etIP);
        etPort = findViewById(R.id.etPort);
        tvMessages = findViewById(R.id.tvMessages);
        speedValTV = findViewById(R.id.speedValTV);
        directionTV = findViewById(R.id.directionTV);
        timeTV = findViewById(R.id.timeTV);
        distanceTV = findViewById(R.id.distanceTV);
        final Button connectBtn = findViewById(R.id.connectBtn);
        final Button btnUpdateVals = findViewById(R.id.getUpdatedVals);
        final Button disconnectBtn = findViewById(R.id.disconnectBtn);

        tvMessages.setVisibility(View.GONE);
        disconnectBtn.setVisibility(View.INVISIBLE);
        btnUpdateVals.setVisibility(View.INVISIBLE);

        btnUpdateVals.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    receiveVehicleData();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        connectBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                tvMessages.setVisibility(View.VISIBLE);
                tvMessages.setText("");
                SERVER_IP = etIP.getText().toString().trim();
                //makes sure the IP address entered is valid based on the regex
                Matcher iPAddrMatcher = IPV4_ADDR_PATTERN.matcher(SERVER_IP);
                if(!iPAddrMatcher.matches())
                {
                    Toast.makeText(getApplicationContext(),"Please enter a valid IP address and port number",
                            Toast.LENGTH_LONG).show();
                }
                else
                {
                    SERVER_PORT = Integer.parseInt(etPort.getText().toString().trim());

                    try
                    {
                        socket = new Socket(SERVER_IP,SERVER_PORT);
                        etIP.setVisibility(View.INVISIBLE);
                        etPort.setVisibility(View.INVISIBLE);
                        connectBtn.setVisibility(View.INVISIBLE);

                        disconnectBtn.setVisibility(View.VISIBLE);
                        tvMessages.setText("Connected to " + SERVER_IP + " at port number " + SERVER_PORT + "\n");
                        btnUpdateVals.setVisibility(View.VISIBLE);

                        din=new DataInputStream(socket.getInputStream());
                        dout=new DataOutputStream(socket.getOutputStream());
                    } catch (IOException e) {
                        Toast.makeText(getApplicationContext(),"Connection to " + SERVER_IP
                                + " was unsuccessful", Toast.LENGTH_LONG).show();
                        e.printStackTrace();
                    }
                }
            }
        });

        disconnectBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    dout.writeUTF("mobile/close");
                    dout.flush();
                    dout.close();
                    din.close();
                    socket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }

                etIP.setVisibility(View.VISIBLE);
                etPort.setVisibility(View.VISIBLE);
                connectBtn.setVisibility(View.VISIBLE);

                disconnectBtn.setVisibility(View.INVISIBLE);
                btnUpdateVals.setVisibility(View.INVISIBLE);
                tvMessages.setVisibility(View.GONE);

                //clear the screen
                speedValTV.setText("");
                directionTV.setText("");
                timeTV.setText("");
                distanceTV.setText("");
            }
        });
    }

    void receiveVehicleData() throws IOException
    {
        dout.writeUTF("mobile/update");
        dout.flush();

        String message = din.readUTF();
        String[] messageArray = message.split(" ");
        for(int i = 0; i < messageArray.length; i++)
        {
            String[] dataType = messageArray[i].split("/");
            switch (dataType[1]) {
                case "speed":
                    int speed = Integer.parseInt(dataType[2]);
                    //set a textview for speed on the screen with the updated value
                    speedValTV.setText("Speed: " + speed + " mph");
                    break;
                case "steering":
                    int direction = Integer.parseInt(dataType[2]);
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
                case "DayTime":
                    int time = Integer.parseInt(dataType[2]);
                    if (time > 1000) {
                        //display that it is currently daytime
                        timeTV.setText("It is currently daytime.");
                    } else
                        //display that it is currently nighttime
                        timeTV.setText("It is currently nighttime.");
                    break;
                case "Distance":
                    long distance = Long.parseLong(dataType[2]);
                    //display current distance in cm
                    distanceTV.setText("Distance: " + distance + " cm");
                    break;
            }
        }
    }
}