/*
 * Decompiled with CFR 0.152.
 * 
 * TransferCase - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.Event;
import java.awt.Label;
import java.awt.TextField;

class TransferCase
extends RatioPanel {
    TextField low;
    Gear outerparent;

    TransferCase(Gear gear, String string) {
        this.outerparent = gear;
        this.low = new TextField(string, 4);
        this.add(new Label("Transfer Case Low: ", 2));
        this.add(this.low);
    }

    public double getvalTransferCase() throws NewEx {
        return this.convertRatio("Transfercase", this.low.getText());
    }

    public void setfocus() {
        this.low.requestFocus();
    }

    public boolean action(Event event, Object object) {
        if (event.target instanceof TextField) {
            this.outerparent.update(this);
            return true;
        }
        return false;
    }
}
