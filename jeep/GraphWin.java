/*
 * Decompiled with CFR 0.152.
 * 
 * GraphWin - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.BorderLayout;
import java.awt.Checkbox;
import java.awt.Event;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.Label;
import java.awt.Panel;

class GraphWin
extends Frame {
    GraphLine graphLine;
    Checkbox lowCheckbox;
    Panel lowPanel;

    GraphWin(int n, int n2) {
        this.setLayout(new BorderLayout());
        this.graphLine = new GraphLine(n, n2);
        this.add("Center", this.graphLine);
        this.resize(600, 480);
    }

    public void setWindowTitle(String string) {
        this.setTitle(string);
    }

    public void setPageTitle(String string) {
        this.add("North", new Label(string, 1));
    }

    public void setYname(String string) {
        this.graphLine.setYname(string);
    }

    public void setNames(String string) {
        this.graphLine.setNames(string);
    }

    public void setXlabel(int n) {
        this.graphLine.setXlabel(n);
    }

    public void setYlabel(int n) {
        this.graphLine.setYlabel(n);
    }

    public void setValues(double d) {
        this.graphLine.setValues(d);
    }

    public void setLow(double d) {
        this.graphLine.setLow(d);
        this.lowPanel = new Panel();
        this.lowPanel.setLayout(new FlowLayout(1));
        this.lowCheckbox = new Checkbox("Low range " + d + ":1");
        this.lowPanel.add(this.lowCheckbox);
        this.add("South", this.lowPanel);
    }

    public void draw() {
        this.graphLine.update();
        this.show();
    }

    public boolean action(Event event, Object object) {
        if (event.target instanceof Checkbox) {
            this.graphLine.goLow();
            this.draw();
            return true;
        }
        return false;
    }

    public boolean handleEvent(Event event) {
        if (event.id == 201) {
            this.dispose();
        }
        return super.handleEvent(event);
    }
}
