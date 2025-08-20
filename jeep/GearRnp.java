/*
 * Decompiled with CFR 0.152.
 * 
 * GearRnp - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.Event;
import java.awt.TextField;

class GearRnp
extends Rnp {
    Gear outerparent;

    GearRnp(Gear gear, String string) {
        super(string);
        this.outerparent = gear;
    }

    public boolean action(Event event, Object object) {
        if (event.target instanceof TextField) {
            this.outerparent.update(this);
            return true;
        }
        return false;
    }
}
