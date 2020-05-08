package com.example.vehicleapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Button connectBttn = findViewById(R.id.connectBttn);
        final Button disconnectBttn = findViewById(R.id.disconnectBttn);
        final TextView ipAddTV = findViewById(R.id.ipAddTV);
        final EditText ipAddET = findViewById(R.id.ipAddET);
        final EditText ipPortET = findViewById(R.id.portET);
        final TextView ipPortTV = findViewById(R.id.portTV);

        final TextView dataFromServerTV = findViewById(R.id.dataFromServerTV);
        final TextView dataTV = findViewById(R.id.dataTV);

        dataFromServerTV.setVisibility(View.INVISIBLE);
        disconnectBttn.setVisibility(View.INVISIBLE);

        connectBttn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String ipAdd = ipAddET.getText().toString();
                int ipPort = Integer.parseInt(ipPortET.getText().toString());

                try {
                    Socket socketConn = new Socket(ipAdd, ipPort);
                    PrintWriter out = new PrintWriter(socketConn.getOutputStream(), true);
                    BufferedReader in = new BufferedReader(new InputStreamReader(socketConn.getInputStream()));
                    out.println("Connected to the app. Your IP address is: "  + ipAdd + " and port number is: " + Integer.toString(ipPort));

                    ipAddTV.setVisibility(View.GONE);
                    ipAddET.setVisibility(View.GONE);
                    ipPortET.setVisibility(View.GONE);
                    ipPortTV.setVisibility(View.GONE);
                    connectBttn.setVisibility(View.GONE);

                    dataFromServerTV.setVisibility(View.VISIBLE);

                    String userInput;
                    while ((userInput = in.readLine()) != null) {
                        dataTV.setText(userInput);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                    Toast.makeText(getApplicationContext(), "Could not connect to server.", Toast.LENGTH_SHORT).show();
                }

            }
        });
    }
}
