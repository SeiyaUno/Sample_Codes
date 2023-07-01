package com.example.healthmastery;

import static android.content.Context.LAYOUT_INFLATER_SERVICE;
import static androidx.core.content.ContextCompat.getSystemService;

import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.ProgressBar;
import android.widget.TextView;

import org.w3c.dom.Text;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link DashboardFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class DashboardFragment extends Fragment {

    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";


    private TextView exerciseSuggestion;
    private TextView foodSuggestion;
    private ProgressBar weeklyProgressBar;
    private int weeklyProgress; // want to get this from database
    private int weeklyProgressMax;

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    public DashboardFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment DashboardFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static DashboardFragment newInstance(String param1, String param2) {
        DashboardFragment fragment = new DashboardFragment();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (getArguments() != null) {
            mParam1 = getArguments().getString(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
//        weeklyProgress = 0; // want this get this from database
//        weeklyProgressMax = 10;
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_dashboard, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        // for progress bar, use setMax() and setProgress()
        weeklyProgressBar = getView().findViewById(R.id.PB_weekly);
        exerciseSuggestion = getView().findViewById(R.id.exerciseSuggestion);
        foodSuggestion = getView().findViewById(R.id.foodSuggestion);

        // we should set the max progress and current progress here
        weeklyProgress = 0; // want this get this from database
        weeklyProgressMax = 10;
        weeklyProgressBar.setProgress(weeklyProgress);
        weeklyProgressBar.setMax(weeklyProgressMax);

        exerciseSuggestion.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ShowRecommendationPopupWindow(v);
            }
        });

        foodSuggestion.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ShowRecommendationPopupWindow(v);
            }
        });

    }

    // we might want to pass String and display it in popup window
    private void ShowRecommendationPopupWindow(View view){
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
                weeklyProgress += 1;
                weeklyProgressBar.setProgress(weeklyProgress);
                popupComplete.dismiss();
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
}