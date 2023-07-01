package com.example.healthmastery;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.List;

public class FoodAdapter extends BaseAdapter {

    Context ctx;
    List<Food> foodList;

    LayoutInflater inflater;

    public FoodAdapter(Context ctx, List<Food> foodList)
    {
        this.ctx = ctx;
        this.foodList = foodList;
        inflater = LayoutInflater.from(ctx);
    }
    @Override
    public int getCount() {
        return foodList.size();
    }

    @Override
    public Object getItem(int position) {
        return foodList.get(position);
    }

    @Override
    public long getItemId(int position) {
        // Return a unique identifier for each item, for example, the position itself
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        convertView = inflater.inflate(R.layout.activity_food_list, null);
        TextView nameView = (TextView) convertView.findViewById(R.id.nameView);
        nameView.setText(foodList.get(position).getName());


        TextView descriptionView = (TextView) convertView.findViewById(R.id.descriptionView);
        descriptionView.setText(foodList.get(position).getDescription());


        TextView caloriesView = (TextView) convertView.findViewById(R.id.caloriesView);
        int iCalories = foodList.get(position).getCalories();
        caloriesView.setText(Integer.toString(iCalories));

        return convertView;
    }
}
