/*
 * Decompiled with CFR 0.152.
 * 
 * SpeedoTire - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.Event;
import java.awt.TextField;

class SpeedoTire
extends Tire {
    Speedo outerparent;

    SpeedoTire(Speedo speedo) {
        this.outerparent = speedo;
    }

    public boolean action(Event event, Object object) {
        if (event.target instanceof TextField) {
            this.outerparent.update(this);
            return true;
        }
        return false;
    }
}
