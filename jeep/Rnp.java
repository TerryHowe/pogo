/*
 * Decompiled with CFR 0.152.
 * 
 * Rnp - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.FlowLayout;
import java.awt.Label;
import java.awt.TextField;

class Rnp
extends RatioPanel {
    TextField rnp;

    Rnp(String string) {
        this.setLayout(new FlowLayout(1));
        this.rnp = new TextField(string, 4);
        this.add(new Label("Enter Ring and Pinion: ", 2));
        this.add(this.rnp);
    }

    public double getvalRnp() throws NewEx {
        return this.convertRatio("Ring and Pinion", this.rnp.getText());
    }

    public void setfocus() {
        this.rnp.requestFocus();
    }
}
