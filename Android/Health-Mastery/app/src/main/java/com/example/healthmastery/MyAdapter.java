package com.example.healthmastery;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;
import java.util.logging.Level;

public class MyAdapter extends RecyclerView.Adapter<MyAdapter.ViewHolder> {

    // 1 Data
    private Context context;
    private ArrayList<LevelModel> levelsList;

    // Master = 0
//    Elitist = 1
//    Warrior = 2
//    Explorer = 3
//    Novice = 4
    private int selected_level;

    private View old_view;

    private OnLevelClick callback;



    // 2 constructor


    public MyAdapter(Context context, ArrayList<LevelModel> levelsList, OnLevelClick listener) {
        this.context = context;
        this.levelsList = levelsList;
        this.callback = listener;
        this.selected_level = -1; // default value
    }

    // 3 View Holder
    public class ViewHolder extends RecyclerView.ViewHolder{
        private ImageView iconImg;
        private TextView levelName;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);

            iconImg = itemView.findViewById(R.id.cardviewImg);
            levelName = itemView.findViewById(R.id.textviewCard);
        }
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.card_level_item,
                parent, false);

        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        LevelModel model = levelsList.get(position);
        holder.levelName.setText(model.getLevelName());
        holder.iconImg.setImageResource(model.getLevelIcon());

        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // set a chosen level and store it somewhere

                if(!levelsList.get(position).getIsSelected()){

                    // disable previously selected one
                    if(selected_level != -1) {
                        levelsList.get(selected_level).setIsSelected(false);
                        old_view.setBackgroundColor(Color.rgb(255, 255, 255));

                    }


                    // set currently seleted one
                    holder.itemView.setBackgroundColor(Color.rgb(255, 179, 15));
//                    view.setBackgroundColor(Color.rgb(255, 179, 15));
                    levelsList.get(position).setIsSelected(true);
                    selected_level = position;
                    callback.onClick(String.valueOf(selected_level));
                    old_view = holder.itemView;

                    // for debug
                    Toast.makeText(context,
                            "" + "You choose: "+ levelsList.get(position).getLevelName() +
                            " " + selected_level,
                            Toast.LENGTH_LONG).show();

                }

            }
        });
    }

    @Override
    public int getItemCount() {
        return levelsList.size();
    }
}
