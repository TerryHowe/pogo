/*
 * Decompiled with CFR 0.152.
 * 
 * Transmission - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.Event;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.Label;
import java.awt.Panel;
import java.awt.TextField;

class Transmission
extends RatioPanel {
    TextField first;
    TextField second;
    TextField third;
    TextField fourth;
    TextField fifth;
    TextField reverse;
    Panel firstPanel;
    Panel secondPanel;
    Panel thirdPanel;
    Panel fourthPanel;
    Panel fifthPanel;
    Panel reversePanel;
    Gear outerparent;
    int curField = 0;
    public static final int FIRST = 0;
    public static final int SECOND = 1;
    public static final int THIRD = 2;
    public static final int FOURTH = 3;
    public static final int FIFTH = 4;
    public static final int REVERSE = 5;
    public static final String[] gearString = new String[]{"1st", "2nd", "3rd", "4th", "5th", "Rev"};

    Transmission(Gear gear, String string, String string2, String string3, String string4, String string5, String string6) {
        this.outerparent = gear;
        this.setLayout(new GridLayout(3, 2));
        this.firstPanel = new Panel();
        this.firstPanel.setLayout(new FlowLayout(2));
        this.first = new TextField(string, 4);
        this.firstPanel.add(new Label(gearString[0], 2));
        this.firstPanel.add(this.first);
        this.add(this.firstPanel);
        this.secondPanel = new Panel();
        this.secondPanel.setLayout(new FlowLayout(0));
        this.second = new TextField(string2, 4);
        this.secondPanel.add(new Label(gearString[1], 2));
        this.secondPanel.add(this.second);
        this.add(this.secondPanel);
        this.thirdPanel = new Panel();
        this.thirdPanel.setLayout(new FlowLayout(2));
        this.third = new TextField(string3, 4);
        this.thirdPanel.add(new Label(gearString[2], 2));
        this.thirdPanel.add(this.third);
        this.add(this.thirdPanel);
        this.fourthPanel = new Panel();
        this.fourthPanel.setLayout(new FlowLayout(0));
        this.fourth = new TextField(string4, 4);
        this.fourthPanel.add(new Label(gearString[3], 2));
        this.fourthPanel.add(this.fourth);
        this.add(this.fourthPanel);
        this.fifthPanel = new Panel();
        this.fifthPanel.setLayout(new FlowLayout(2));
        this.fifth = new TextField(string5, 4);
        this.fifthPanel.add(new Label(gearString[4], 2));
        this.fifthPanel.add(this.fifth);
        this.add(this.fifthPanel);
        this.reversePanel = new Panel();
        this.reversePanel.setLayout(new FlowLayout(0));
        this.reverse = new TextField(string6, 4);
        this.reversePanel.add(new Label(gearString[5], 2));
        this.reversePanel.add(this.reverse);
        this.add(this.reversePanel);
    }

    public double getvalfirst() throws NewEx {
        return this.convertRatio("Transmission: first", this.first.getText());
    }

    public double getvalsecond() throws NewEx {
        return this.convertRatio("Transmission: second", this.second.getText());
    }

    public double getvalthird() throws NewEx {
        return this.convertRatio("Transmission: third", this.third.getText());
    }

    public double getvalfourth() throws NewEx {
        return this.convertRatio("Transmission: fourth", this.fourth.getText());
    }

    public double getvalfifth() throws NewEx {
        return this.convertRatio("Transmission: fifth", this.fifth.getText());
    }

    public double getvalreverse() throws NewEx {
        return this.convertRatio("Transmission: reverse", this.reverse.getText());
    }

    public void setfocus(int n) {
        this.curField = n;
        this.setfocus();
    }

    public boolean setfocus() {
        switch (this.curField) {
            case 0: {
                this.first.requestFocus();
                break;
            }
            case 1: {
                this.second.requestFocus();
                break;
            }
            case 2: {
                this.third.requestFocus();
                break;
            }
            case 3: {
                this.fourth.requestFocus();
                break;
            }
            case 4: {
                this.fifth.requestFocus();
                break;
            }
            case 5: {
                this.reverse.requestFocus();
                break;
            }
        }
        this.curField = (this.curField + 1) % 6;
        return this.curField == 1;
    }

    public boolean action(Event event, Object object) {
        if (event.target instanceof TextField) {
            this.outerparent.update(this);
            return true;
        }
        return false;
    }
}
