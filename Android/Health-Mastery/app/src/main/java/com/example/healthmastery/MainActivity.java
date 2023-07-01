package com.example.healthmastery;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    private Button startBtn, skipBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        want to call get started screen if the user has not created a profile
//        if a user already have a profile, move to main dashboard screen
//        setContentView(R.layout.activity_main);
        setContentView(R.layout.activity_get_started);

        startBtn = findViewById(R.id.startBtn);
        skipBtn = findViewById(R.id.skipBtn);


        startBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view){

                Intent i = new Intent(getApplicationContext(),
                        EnterInitialInfo.class);

                startActivity(i);
            }
        });

        skipBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view){

                Intent i = new Intent(getApplicationContext(),
                        LoginPage.class);

                startActivity(i);
            }
        });
    }
}