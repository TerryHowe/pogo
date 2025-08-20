/*
 * Decompiled with CFR 0.152.
 * 
 * HelpScreen - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.BorderLayout;
import java.awt.Label;
import java.awt.Panel;
import java.awt.TextArea;
import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;

class HelpScreen
extends Panel
implements Runnable {
    TextArea area;
    Label topLabel;
    URL saveCb;
    URL theURL;
    HelpWin gh;

    HelpScreen(HelpWin helpWin, URL uRL, int n, int n2) {
        this.gh = helpWin;
        this.saveCb = uRL;
        this.area = new TextArea("", n, n2);
        this.topLabel = new Label("Getting text ... main", 1);
        this.setLayout(new BorderLayout());
        this.add("North", this.topLabel);
        this.add("Center", this.area);
    }

    public boolean setURL(String string) {
        try {
            this.theURL = new URL(this.saveCb, string);
        }
        catch (MalformedURLException malformedURLException) {
            this.add("North", new Label("Bad URL: " + this.theURL, 1));
            return false;
        }
        this.topLabel.setText("Getting text ... " + string);
        this.show();
        return true;
    }

    public void run() {
        StringBuffer stringBuffer = new StringBuffer();
        try {
            String string;
            InputStream inputStream = this.theURL.openStream();
            DataInputStream dataInputStream = new DataInputStream(new BufferedInputStream(inputStream));
            while ((string = dataInputStream.readLine()) != null) {
                stringBuffer.append(string + "\n");
            }
            inputStream.close();
        }
        catch (IOException iOException) {
            stringBuffer.append("\n***\n");
            stringBuffer.append("*** Error reading: " + iOException.getMessage());
            stringBuffer.append("\n***\n");
        }
        this.area.setText(stringBuffer.toString());
        this.topLabel.setText("");
        this.show();
    }
}
