package com.example.healthmastery;

public class LevelModel {

    private String levelName;
    private int levelIcon;

    private boolean isSelected;

    public LevelModel(String levelName, int levelIcon) {
        this.levelName = levelName;
        this.levelIcon = levelIcon;
        this.isSelected = false;
    }

    public String getLevelName() {
        return levelName;
    }

    public void setLevelName(String levelName) {
        this.levelName = levelName;
    }

    public int getLevelIcon() {
        return levelIcon;
    }

    public void setLevelIcon(int levelIcon) {
        this.levelIcon = levelIcon;
    }

    public boolean getIsSelected() {
        return isSelected;
    }

    public void setIsSelected(boolean selected) {
        isSelected = selected;
    }
}
