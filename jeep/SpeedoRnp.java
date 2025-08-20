/*
 * Decompiled with CFR 0.152.
 * 
 * SpeedoRnp - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.Event;
import java.awt.TextField;

class SpeedoRnp
extends Rnp {
    Speedo outerparent;

    SpeedoRnp(Speedo speedo, String string) {
        super(string);
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
