package com.example.healthmastery;

import com.google.android.material.textfield.TextInputLayout;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.ValueEventListener;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.Address;
import android.location.Geocoder;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.Manifest;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import com.example.healthmastery.R;
import com.example.healthmastery.EnterInitialInfo;
import com.example.healthmastery.Food;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.PopupWindow;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Objects;
import java.util.Random;

enum ListMode {
    food,
    exercise,
    water
}

public class HomePage extends AppCompatActivity implements LocationListener {

    TextView welcomeBanner;
    String testCity = "Irvine";

    String userName = "Welcome ";
    private  String levels;
    String userWeight = "";
    String userEmail = "";
    String drinkWaterText = "Amount of water to drink: ";

    double amountOfWaterToDrink = 0;
    int userWeightInt = 0;

    TextView weatherTextViewRs, drinkingWaterBanner;
    //https://api.openweathermap.org/data/2.5/weather?q=Irvine&appid=ed94e3dd0c259b3cf123001c2f3cc6b4
    private final String weatherAPI = "https://api.openweathermap.org/data/2.5/weather?";
    private final String APIKEY = "ed94e3dd0c259b3cf123001c2f3cc6b4";
    DecimalFormat df = new DecimalFormat("#.##");

    private ListView workoutsListView, foodsListView, userInfoListView;

    private FoodAdapter foodsAdapter;
    private UserInformationAdapter userInfoAdapter;
    private WorkoutAdapter workoutsAdapter;

    private List<Workout> workoutsList;
    private List<Food> foodsList;
    private List<UserInformation> userInfoList;
    private ArrayList<String> badWeather = new ArrayList<>();
    private String currWeather = "";
    private boolean flexibility = true;

    private ProgressBar weeklyProgressBar;
    private int weeklyProgress; // want to get this from database
    private int weeklyProgressMax;
    private ImageView waterIcon;

    private TextInputLayout updated_weight;

    private static final int PERMISSION_REQUEST_CODE = 1;
    private LocationManager locationManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_page);

        welcomeBanner = findViewById(R.id.welcomeBanner);
        workoutsListView = findViewById(R.id.workoutsListView);
        foodsListView = findViewById(R.id.foodTEMP);
        userInfoListView = findViewById(R.id.userInfoDisplay);


        weatherTextViewRs = findViewById(R.id.weatherRs); // Added this line
        drinkingWaterBanner = findViewById(R.id.waterDrinking);

        weeklyProgressBar = findViewById(R.id.PB_weekly);
        weeklyProgress = 0; // want this get this from database
        weeklyProgressMax = 1;
        weeklyProgressBar.setProgress(weeklyProgress);
//        weeklyProgressBar.setMax(weeklyProgressMax);

        waterIcon = findViewById(R.id.waterIcon);
        //empty passByValues variables will show up as Null

        Intent intent = getIntent();
        userName = userName + intent.getStringExtra("userNameKey") + "!";
        welcomeBanner.setText(userName);

        badWeather.add("Thunderstorm");
        badWeather.add("Drizzle");
        badWeather.add("Rain");
        badWeather.add("Snow");
        badWeather.add("Haze");
        badWeather.add("Smoke");

        userName = intent.getStringExtra("userNameKey");
      double latitude = 33.132;
      double longitude = -117.123;
      if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) !=
        PackageManager.PERMISSION_GRANTED &&
        ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) !=
          PackageManager.PERMISSION_GRANTED) {
        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_FINE_LOCATION,
          Manifest.permission.ACCESS_COARSE_LOCATION}, PERMISSION_REQUEST_CODE);
      } else {
        // Permission already granted
        //initializeLocation();
      }
      try {
        LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        @SuppressLint("MissingPermission")  Location lastKnownLocation = locationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);

        if (lastKnownLocation != null) {
          latitude = lastKnownLocation.getLatitude();
          longitude = lastKnownLocation.getLongitude();
          testCity = getAddress(latitude,longitude);

          Geocoder geocoder = new Geocoder(HomePage.this, Locale.getDefault());
          List<Address> addresses = geocoder.getFromLocation(latitude, longitude, 1);

          if (!addresses.isEmpty()) {
            String city = addresses.get(0).getAddressLine(1);
            //getAddress(latitude,longitude);
            if (city != null) {
              // Use the city value as needed
              testCity = city;
            }
          }
        }
      } catch (IOException e) {
        e.printStackTrace();
      }

        String tempUrl = weatherAPI + "q=" + testCity + "&appid=" + APIKEY;

        StringRequest stringRequest = new StringRequest(Request.Method.POST, tempUrl, new Response.Listener<String>() {
            public void onResponse(String response) {
                String weatherResult = "";
                try {
                    JSONObject jsonResponse = new JSONObject(response);
                    JSONArray jsonArray = jsonResponse.getJSONArray("weather");
                    JSONObject jsonObjectWeather = jsonArray.getJSONObject(0);
                    currWeather = jsonObjectWeather.getString("main");
                    //currWeather = "Rain";
                    JSONObject jsonObjectMain = jsonResponse.getJSONObject("main");
                    double temp = (jsonObjectMain.getDouble("temp") - 273.15) * 1.8 + 32 ;
                    JSONObject jsonObjectSys = jsonResponse.getJSONObject("sys");
                    String countryName = jsonObjectSys.getString("country");
                    String cityName = jsonResponse.getString("name");
                    weatherResult += "Current weather of " + cityName + " (" + countryName + ")"
                            + "\n Temp: " + df.format(temp) + " °F"
                            + "\n Current Weather: " + currWeather;

                    weatherTextViewRs.setText(weatherResult);
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
        }, new Response.ErrorListener(){
            @Override
            public void onErrorResponse(VolleyError error) {
                Toast.makeText(getApplicationContext(), error.toString().trim(), Toast.LENGTH_SHORT).show();
            }
        });
        RequestQueue requestQueue = Volley.newRequestQueue(getApplicationContext());
        requestQueue.add(stringRequest);

        retrieveAndDisplayInfo(userName);
//      retrieveWorkouts(Integer.parseInt(levels));
//      retrieveAndDisplayFoods(Integer.parseInt(levels));
//      retrieveAndDisplayFoods(userName);

        workoutsListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                ShowRecommendationPopupWindow(view, position,ListMode.exercise);
            }
        });

        foodsListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                ShowRecommendationPopupWindow(view, position, ListMode.food);
            }
        });

        waterIcon.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ShowRecommendationPopupWindow(v, 0, ListMode.water);
            }
        });

    }

  private String getAddress(double lats, double lons) {

    Geocoder geocoder;
    double lat = lats;
    double lon = lons;
    geocoder = new Geocoder(this, Locale.getDefault());
    List<android.location.Address> addresses = null;
    try {
      addresses = geocoder.getFromLocation(lat, lon, 1);
    } catch (IOException e) {

      e.printStackTrace();
    }

    if (addresses != null) {

      String address = addresses.get(0).getAddressLine(0);
      String city = addresses.get(0).getLocality();
      String state = addresses.get(0).getAdminArea();
      String country = addresses.get(0).getCountryName();
      String postalCode = addresses.get(0).getPostalCode();
      String knownName = addresses.get(0).getFeatureName();

      return city;
    } else {
      return "failed";
    }


  }

    private void retrieveAndDisplayInfo(String userName)
    {
        FirebaseDatabase firebaseDatabase = FirebaseDatabase.getInstance();
        DatabaseReference usersRef = firebaseDatabase.getReference("users");

        usersRef.orderByChild("userName").equalTo(userName).addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                userInfoList = new ArrayList<>();
                for (DataSnapshot userInfoSnapshot : snapshot.getChildren()) {
                    String userRealName = userInfoSnapshot.child("name").getValue(String.class);
                    String userName = userInfoSnapshot.child("userName").getValue(String.class);
                    String caloriesBurned = userInfoSnapshot.child("caloriesBurned").getValue(String.class);
                    String userLevel = userInfoSnapshot.child("levels").getValue(String.class);
                    String userWeight = userInfoSnapshot.child("weight").getValue(String.class);

                    switch(userLevel) {
                        case "1":
                            userLevel = "5";
                            break;
                        case "2":
                            userLevel = "4";
                            break;
                        case "3":
                            userLevel = "3";
                            break;
                        case "4":
                            userLevel = "2";
                            break;
                        case "5":
                            userLevel = "1";
                            break;
                    }

                    int theLevel = Integer.parseInt(userLevel);
                    String userRealNameInfo = "Name: " + userRealName;
                    String userWeightInfo = "Weight: " + userWeight + " lbs";
                    String levelInfo = "Level: " + userLevel;
                    String caloriesBurnInfo = "Calories to Burn: " + caloriesBurned + " calories.";

                    UserInformation currentUser = new UserInformation(userWeightInfo,userName,caloriesBurnInfo, levelInfo, userWeight);
                    userInfoList.add(currentUser);
                    userInfoAdapter = new UserInformationAdapter(HomePage.this,  userInfoList);
                    userInfoListView.setAdapter(userInfoAdapter);

                    userWeightInt =  Integer.parseInt(userWeight);
                    amountOfWaterToDrink = userWeightInt * (0.66);
                    drinkWaterText = drinkWaterText + String.valueOf(df.format(amountOfWaterToDrink)) + " ounces";
                    drinkingWaterBanner.setText(drinkWaterText);
                    drinkWaterText = "Amount of water to drink: ";



                    if(badWeather.contains(currWeather))
                        retrieveWorkouts(theLevel, false);
                    else
                        retrieveWorkouts(theLevel, true);
                    retrieveAndDisplayFoods(theLevel);
                }


            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });

    }

    public List<Food> determineFoodsToDisplay(int lvl, List<Food> foodList)
    {
        List<Food> returnList = new ArrayList<Food>();
        Random random = new Random();
        int randomIndex;

        Food foodToAdd;

        int numOfFoods = 0;
        if(lvl == 5 )
            numOfFoods = 3;
        else if(lvl == 4 || lvl == 3)
            numOfFoods = 2;
        else if(lvl == 2 || lvl == 1)
            numOfFoods = 1;


        for(int i = 5; i > 5 - numOfFoods; i--)
        {// Generate a random number between 0 (inclusive) and listSize (exclusive)
            randomIndex = random.nextInt(foodList.size());
            foodToAdd = foodList.get(randomIndex);

            if(returnList.contains(foodToAdd))//if the food already exists dont add it
            {
                i++;
                continue;
            }
            else {//add the food
                returnList.add(foodToAdd);
            }
        }

        weeklyProgressMax += returnList.size();
        weeklyProgressBar.setMax(weeklyProgressMax);
        return returnList;
    }
    private void retrieveAndDisplayFoods(int levels)
    {
        FirebaseDatabase firebaseDatabase = FirebaseDatabase.getInstance();
        DatabaseReference foodsRef = firebaseDatabase.getReference("foods");


        //count used to determine how many foods to add
        int count = 0;
        foodsRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                foodsList = new ArrayList<>();
                for (DataSnapshot foodSnapshot : snapshot.getChildren()) {
                    String foodName = foodSnapshot.child("Name").getValue(String.class);
                    String foodDesc = foodSnapshot.child("Description").getValue(String.class);
                    Integer foodCalories = foodSnapshot.child("Calorie Count").getValue(Integer.class);

                    Food currentFood = new Food(foodName,foodDesc,foodCalories);
                    foodsList.add(currentFood);

                }
                List<Food> displayList = determineFoodsToDisplay(levels, foodsList);

                foodsAdapter = new FoodAdapter(HomePage.this,  displayList);
                foodsListView.setAdapter(foodsAdapter);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });
    }

    public List<Workout> determineWorkoutsToDisplay(int lvl, boolean flexibility, List<Workout> workoutsList)
    {
        int numOfWorkouts = 0;
        Workout workoutToAdd;

        //list of workouts to be returned
        List<Workout> returnList = new ArrayList<Workout>();;

        //first determine how many workouts user must complete that day
        if(lvl == 5 )
            numOfWorkouts = 3;
        else if(lvl == 4 || lvl == 3)
            numOfWorkouts = 2;
        else if(lvl == 2 || lvl == 1)
            numOfWorkouts = 1;

        Random random = new Random();
        int randomIndex;

        for(int i = 5; i > 5 - numOfWorkouts ; i--)
        {// Generate a random number between 0 (inclusive) and listSize (exclusive)
            randomIndex = random.nextInt(workoutsList.size());
            workoutToAdd = workoutsList.get(randomIndex);

            if(returnList.contains(workoutToAdd))//if the workout already exists dont add it
            {
                i++;
                continue;
            }//check to see if meets the flexibility
            else if(workoutToAdd.getFlexibility() != flexibility){
                i++;
                continue;//this is making sure that the workout can be done indoor or outdoor
            }
            else{//add the workout
                returnList.add(workoutToAdd);
            }
        }

        weeklyProgressMax += returnList.size();
        Log.e("TEST", "IN workwout "+ weeklyProgressMax);
        return returnList;
    }

    private void retrieveWorkouts(int lvl, boolean theFlexibility) {
        FirebaseDatabase firebaseDatabase = FirebaseDatabase.getInstance();
        DatabaseReference workoutsRef = firebaseDatabase.getReference("workouts");



            //to do: take into account of bad weather
        workoutsRef.orderByChild("level").equalTo(lvl).addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                workoutsList = new ArrayList<>();
                for (DataSnapshot workoutSnapshot : dataSnapshot.getChildren()) {
                    String workoutName = workoutSnapshot.child("name").getValue(String.class);
                    Integer workoutLevel =  workoutSnapshot.child("level").getValue(Integer.class);
                    Integer workoutCalories = workoutSnapshot.child("calories").getValue(Integer.class);
                    Boolean workoutFlexibility = workoutSnapshot.child("flexibility").getValue(Boolean.class);
                    String workoutDesc = workoutSnapshot.child("instructions").getValue(String.class);

                    //for cases where weather is bad
                    if(!flexibility)
                    {
                        if(workoutFlexibility)
                        {
                            Workout currentWorkout = new Workout(workoutName,workoutLevel,workoutCalories,workoutFlexibility,workoutDesc);
                            workoutsList.add(currentWorkout);
                        }
                        else {
                            continue;
                        }
                    }
                    else
                    {
                        Workout currentWorkout = new Workout(workoutName,workoutLevel,workoutCalories,workoutFlexibility,workoutDesc);
                        workoutsList.add(currentWorkout);
                    }




                }
                List<Workout> displayList = determineWorkoutsToDisplay(lvl, theFlexibility, workoutsList);

                workoutsAdapter = new WorkoutAdapter(HomePage.this,  displayList);
                workoutsListView.setAdapter(workoutsAdapter);

            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                Toast.makeText(getApplicationContext(), "Failed to retrieve workouts", Toast.LENGTH_SHORT).show();
            }
        });
    }

    // for popup window to mark tasks as completed
    private void ShowRecommendationPopupWindow(View view, int position, ListMode mode){
        LayoutInflater inflater = (LayoutInflater)
                view.getContext().getSystemService(view.getContext().LAYOUT_INFLATER_SERVICE);

        View popupWindow = inflater.inflate(R.layout.popup_complete, null);
        int width = LinearLayout.LayoutParams.WRAP_CONTENT;
        int height = ViewGroup.LayoutParams.WRAP_CONTENT;
        boolean focusable = false; // controls if touching outside of popup closes it
        final PopupWindow popupComplete = new PopupWindow(popupWindow,
                width, height, focusable);
        popupComplete.setElevation(50.0f);  //show shadow

        popupComplete.showAtLocation(view, Gravity.CENTER, 0, 0);

        dimBackground(popupComplete);

        // set up button actions
        Button cmp_btn = popupComplete.getContentView().findViewById(R.id.popup_cmp);
        Button close_btn = popupComplete.getContentView().findViewById(R.id.popup_close);

        cmp_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // call function to increase progress
                // also want to delete the list selected
                if(mode == ListMode.food) {
                    foodsAdapter.foodList.remove(position);
                    foodsListView.invalidateViews();
                }
                else if(mode == ListMode.exercise) {
                    workoutsAdapter.workoutList.remove(position);
                    workoutsListView.invalidateViews();
                }
                else if(mode == ListMode.water) {
                    drinkingWaterBanner.setText("You had enough water today.");
                }

                weeklyProgress += 1;
                weeklyProgressBar.setProgress(weeklyProgress);
                popupComplete.dismiss();

                // when user completes all tasks
                if(weeklyProgress == weeklyProgressMax) {
                    weeklyProgress = 0;
                    weeklyProgressMax = 1;
                    weeklyProgressBar.setProgress(weeklyProgress);
                    drinkWaterText = "Amount of water to drink: ";
                    askForWeight(view);
                    retrieveAndDisplayInfo(userName);
                }
            }
        });

        close_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                popupComplete.dismiss();
            }
        });
    }

    private void dimBackground(PopupWindow popupWindow) {
        View view = popupWindow.getContentView().getRootView();
        Context context = popupWindow.getContentView().getContext();
        WindowManager windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        WindowManager.LayoutParams layoutParams = (WindowManager.LayoutParams) view.getLayoutParams();

        layoutParams.flags = WindowManager.LayoutParams.FLAG_DIM_BEHIND;
        layoutParams.dimAmount = 0.5f;
        windowManager.updateViewLayout(view, layoutParams);
    }

    private void askForWeight(View view) {
        LayoutInflater inflater = (LayoutInflater)
                view.getContext().getSystemService(view.getContext().LAYOUT_INFLATER_SERVICE);

        View popupWindow = inflater.inflate(R.layout.popup_weight, null);
        int width = LinearLayout.LayoutParams.WRAP_CONTENT;
        int height = ViewGroup.LayoutParams.WRAP_CONTENT;
        boolean focusable = false; // controls if touching outside of popup closes it
        final PopupWindow popupWeight = new PopupWindow(popupWindow,
                width, height, focusable);
        popupWeight.setElevation(50.0f);



        updated_weight = popupWeight.getContentView().findViewById(R.id.updatedWeight);

        Button weight_submit = popupWeight.getContentView().findViewById(R.id.weightSubmit);

        popupWeight.showAtLocation(view, Gravity.CENTER, 0, 0);

        dimBackground(popupWeight);
        weight_submit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                FirebaseDatabase firebaseDatabase = FirebaseDatabase.getInstance();
                DatabaseReference childReference = firebaseDatabase.getReference().child("users").child(userName).child("weight");
                String weightDummy = Objects.requireNonNull(updated_weight.getEditText()).getText().toString();
                childReference.setValue(weightDummy);

                popupWeight.dismiss();
            }
        });
    }

  @Override
  public void onLocationChanged(@NonNull Location location) {
    try {
      Geocoder geocoder = new Geocoder(HomePage.this, Locale.getDefault());
      List<Address> addresses = geocoder.getFromLocation(location.getLatitude(),location.getLongitude(),1);
      testCity = addresses.get(0).getLocality();


    }catch (Exception e){
      e.printStackTrace();
    }
  }
}

