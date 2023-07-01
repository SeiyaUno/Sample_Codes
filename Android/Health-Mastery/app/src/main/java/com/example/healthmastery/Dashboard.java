package com.example.healthmastery;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.MenuItem;

import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.google.android.material.navigation.NavigationBarView;

public class Dashboard extends AppCompatActivity implements NavigationBarView.OnItemSelectedListener {

    BottomNavigationView btmNavigationView;

    DashboardFragment dashboardFragment = new DashboardFragment();
    ProfileFragment profileFragment = new ProfileFragment();
    SettingFragment settingFragment = new SettingFragment();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dashboard2);

        btmNavigationView = findViewById(R.id.bottomNavigationView);
        btmNavigationView.setOnItemSelectedListener(this);
        btmNavigationView.setSelectedItemId(R.id.main_home);
    }

    // We only need two fragments
    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem item) {
//        if(item.getItemId() == R.id.profile){
//            getSupportFragmentManager().beginTransaction().replace(R.id.flFlagment, profileFragment).commit();
//            return true;
//        }
//        else
        if(item.getItemId() == R.id.main_home){
            getSupportFragmentManager().beginTransaction().replace(R.id.flFlagment, dashboardFragment).commit();
            return true;
        }
        else if(item.getItemId() == R.id.settings){
            getSupportFragmentManager().beginTransaction().replace(R.id.flFlagment, settingFragment).commit();
            return true;
        }

        return false;
    }
}