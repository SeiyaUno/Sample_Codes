package com.example.healthmastery;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.google.android.material.textfield.TextInputLayout;
import com.google.firebase.database.FirebaseDatabase;

import java.util.Objects;

public class LoginPage extends AppCompatActivity {

    TextInputLayout userNameInput;
    Button submitButton;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        want to call get started screen if the user has not created a profile
//        if a user already have a profile, move to main dashboard screen
//        setContentView(R.layout.activity_main);
        setContentView(R.layout.activity_login_page);

        userNameInput = findViewById(R.id.nameInput);
        submitButton = findViewById(R.id.submit_info_button);

        submitButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view){


            //child is like a primary key, so nameInput is the primary key
            String userName =  Objects.requireNonNull(userNameInput.getEditText()).getText().toString();

            Intent i = new Intent(getApplicationContext(), HomePage.class);
                //this is to pass the userName variable to the next activity
            i.putExtra("userNameKey" , userName);
            startActivity(i);
            }
        });






    }
}