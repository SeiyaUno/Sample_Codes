package com.example.healthmastery;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;


import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;


import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Toast;

import com.google.android.material.textfield.TextInputLayout;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

import java.util.ArrayList;
import java.util.Objects;

public class EnterInitialInfo extends AppCompatActivity implements OnLevelClick{


    ArrayList<LevelModel> levelsList;

    RecyclerView recyclerView;

    MyAdapter adapter;
//    private Button completeBtn;
    FirebaseDatabase rootNode;
    DatabaseReference ref;

    TextInputLayout userName, name, email, age,weight, height, caloriesIntake, caloriesBurned;
    String gender;
    Button submitButton, skipButton;
    public static final String userNameKey = "";
    public static final String userLevel = "";

    String selectedLevel;


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_enter_initial_info);

        recyclerView = findViewById(R.id.levelsList);
//        completeBtn = findViewById(R.id.init_info_cmp_btn);

        levelsList = new ArrayList<>();
        levelsList.add(new LevelModel("Master",R.drawable.master));
        levelsList.add(new LevelModel("Elitist",R.drawable.elitist));
        levelsList.add(new LevelModel("Warrior",R.drawable.warrior));
        levelsList.add(new LevelModel("Explorer",R.drawable.explorer));
        levelsList.add(new LevelModel("Novice",R.drawable.novice));

        adapter = new MyAdapter(this, levelsList, this);
        RecyclerView.LayoutManager layoutManager = new LinearLayoutManager(this,
                LinearLayoutManager.VERTICAL, false);

        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(adapter);


        RadioGroup radioGroup = findViewById(R.id.genderButtonGroup);

        radioGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener()
        {
            @SuppressLint("NonConstantResourceId")
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                // checkedId is the RadioButton selected
                if (checkedId == R.id.btn_male) {
                    gender = "Male";
                } else if (checkedId == R.id.btn_female) {
                    gender = "Female";
                }
            }
        });



        userName = findViewById(R.id.userNameInput);
        name = findViewById(R.id.nameInput);
        email = findViewById(R.id.emailInput);
        age = findViewById(R.id.ageInput);
        weight = findViewById(R.id.weightInput);
        height = findViewById(R.id.heightInput);
        caloriesIntake = findViewById(R.id.calorieIntakeInput);
        caloriesBurned = findViewById(R.id.calorieBurnedInput);
        submitButton = findViewById(R.id.init_info_next_btn);


        submitButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view){

                Log.w("TEST", selectedLevel);

                //getting firebase reference and choosing which table to insert data
                rootNode = FirebaseDatabase.getInstance();
                ref = rootNode.getReference("users");


                //getting the text input from all the fields
               String userNameInput = Objects.requireNonNull(userName.getEditText()).getText().toString();
               String nameInput = Objects.requireNonNull(name.getEditText()).getText().toString();
               String emailInput = Objects.requireNonNull(email.getEditText()).getText().toString();
               String ageInput = Objects.requireNonNull(age.getEditText()).getText().toString();
               String weightInput = Objects.requireNonNull(weight.getEditText()).getText().toString();
               String heightInput = Objects.requireNonNull(height.getEditText()).getText().toString();
               String caloriesIntakeInput = Objects.requireNonNull(caloriesIntake.getEditText()).getText().toString();
               String caloriesBurnedInput = Objects.requireNonNull(caloriesBurned.getEditText()).getText().toString();



               UserHelper helper = new UserHelper(userNameInput, nameInput,emailInput, ageInput, weightInput, heightInput, caloriesIntakeInput, caloriesBurnedInput, gender, selectedLevel);

               //child is like a primary key, so nameInput is the primary key
               ref.child(userNameInput).setValue(helper);

               Intent i = new Intent(getApplicationContext(), HomePage.class);
               //this is to pass the userName variable to the next activity
               i.putExtra("userNameKey" , userNameInput);
               startActivity(i);
            }
        });
    }

    @Override
    public void onClick(String level) {
        selectedLevel = level;
        int tempValue = Integer.parseInt(selectedLevel);
        tempValue ++;
        selectedLevel = String.valueOf(tempValue);
    }
}