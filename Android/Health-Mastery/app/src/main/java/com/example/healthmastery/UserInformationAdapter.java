package com.example.healthmastery;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.List;

public class UserInformationAdapter extends BaseAdapter {

    Context ctx;
    List<UserInformation> userInfoList;

    LayoutInflater inflater;

    public UserInformationAdapter(Context ctx, List<UserInformation> userInfoList)
    {
        this.ctx = ctx;
        this.userInfoList = userInfoList;
        inflater = LayoutInflater.from(ctx);
    }
    @Override
    public int getCount() {
        return userInfoList.size();
    }

    @Override
    public Object getItem(int position) {
        return userInfoList.get(position);
    }

    @Override
    public long getItemId(int position) {
        // Return a unique identifier for each item, for example, the position itself
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        convertView = inflater.inflate(R.layout.activity_user_information_lists, null);


        TextView userName = (TextView) convertView.findViewById(R.id.userNameView);
        userName.setText(userInfoList.get(position).getUserName());

        TextView nameView = (TextView) convertView.findViewById(R.id.nameView);
        nameView.setText(userInfoList.get(position).getUserRealName());

        TextView calorieView = (TextView) convertView.findViewById(R.id.caloriesView);
        calorieView.setText(userInfoList.get(position).getUserCalories());

        TextView levels = (TextView) convertView.findViewById(R.id.levelsView);
        levels.setText(userInfoList.get(position).getUserLevel());



//        TextView caloriesView = (TextView) convertView.findViewById(R.id.caloriesView);
//        String iCalories = userInfoList.get(position).getUserCalories();
//        caloriesView.setText(Integer.toString(iCalories));

        return convertView;
    }
}

