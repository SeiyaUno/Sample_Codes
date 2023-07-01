package com.example.healthmastery;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.List;

public class WorkoutAdapter extends BaseAdapter {

    Context ctx;
    List<Workout> workoutList;

    LayoutInflater inflater;

    public WorkoutAdapter(Context ctx, List<Workout> workoutList) {
        this.ctx = ctx;
        this.workoutList = workoutList;
        inflater = LayoutInflater.from(ctx);
    }

    @Override
    public int getCount() {
        return workoutList.size();
    }

    @Override
    public Object getItem(int position) {
        return workoutList.get(position);
    }

    @Override
    public long getItemId(int position) {
        // Return a unique identifier for each item, for example, the position itself
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        convertView = inflater.inflate(R.layout.activity_workout_list, null);
        TextView nameView = (TextView) convertView.findViewById(R.id.nameView);
        nameView.setText(workoutList.get(position).getName());


        TextView descriptionView = (TextView) convertView.findViewById(R.id.descriptionView);
        descriptionView.setText(workoutList.get(position).getDescription());


        TextView caloriesView = (TextView) convertView.findViewById(R.id.caloriesView);
        int iCalories = workoutList.get(position).getCalories();
        caloriesView.setText(Integer.toString(iCalories));

        return convertView;
    }
}
