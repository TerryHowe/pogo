/*
 * Decompiled with CFR 0.152.
 * 
 * HelpWin - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Event;
import java.awt.Frame;
import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.MenuItem;
import java.net.URL;

class HelpWin
extends Frame {
    HelpScreen helpScreen;
    Thread helpThread;
    String[] options;
    String[] files;
    int count;
    MenuBar mb;
    Menu mu;
    static final int MAXHELPX = 550;
    static final int MAXHELPY = 400;

    HelpWin(URL uRL, String[] stringArray, String[] stringArray2, int n) {
        this.setTitle("Gear and Tire Help");
        this.resize(550, 400);
        this.setLayout(new BorderLayout());
        this.mu = new Menu("Help");
        this.count = n;
        this.files = new String[this.count];
        this.options = new String[this.count];
        int n2 = 0;
        while (n2 < this.count) {
            this.files[n2] = new String(stringArray2[n2]);
            this.options[n2] = new String(stringArray[n2]);
            this.mu.add(new MenuItem(stringArray[n2]));
            ++n2;
        }
        this.helpThread = null;
        this.helpScreen = new HelpScreen(this, uRL, 550, 400);
        this.add("Center", this.helpScreen);
        this.mb = new MenuBar();
        this.setMenuBar(this.mb);
        this.mb.add(this.mu);
        this.mb.setHelpMenu(this.mu);
    }

    public void display() {
        this.startThread(0);
    }

    public void display(String string) {
        int n = 0;
        while (n < this.count) {
            if (this.options[n].equals(string)) {
                this.startThread(n);
                return;
            }
            ++n;
        }
    }

    void startThread(int n) {
        if (this.helpThread != null) {
            this.helpThread.stop();
        }
        this.helpThread = new Thread(this.helpScreen);
        if (!this.helpScreen.setURL(this.files[n])) {
            this.show();
            return;
        }
        this.helpThread.start();
        this.show();
    }

    public boolean handleEvent(Event event) {
        if (event.id == 201) {
            ((Component)this).hide();
        }
        return super.handleEvent(event);
    }

    public boolean action(Event event, Object object) {
        if (event.target instanceof MenuItem) {
            this.display((String)object);
            return true;
        }
        return false;
    }
}
