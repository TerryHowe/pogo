/*
 * Decompiled with CFR 0.152.
 * 
 * GraphLine - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;

class GraphLine
extends Canvas {
    int[] xlabels;
    int[] ylabels;
    String yName;
    String[] names;
    double mod;
    double low;
    double[][] values;
    double decode = 1.0;
    int valuesNum;
    int namesNum;
    int curName;
    int curValue;
    int curXlabel;
    int curYlabel;
    static final int MINAXE = 40;
    static final int MINAYE = 10;
    static final int MAXAXE = 500;
    static final int MAXAYE = 300;
    static final int AXE = 80;
    static final int AYE = 280;

    GraphLine(int n, int n2) {
        this.valuesNum = n2;
        this.namesNum = n;
        this.values = new double[n][n2];
        this.names = new String[n];
        this.xlabels = new int[n2];
        this.ylabels = new int[n2];
        int n3 = 0;
        while (n3 < this.valuesNum) {
            this.ylabels[n3] = 0;
            ++n3;
        }
        this.curName = -1;
        this.curValue = -1;
        this.curXlabel = -1;
        this.curYlabel = -1;
        this.low = 1.0;
        this.mod = 1.0;
    }

    public void setYname(String string) {
        this.yName = new String(string);
    }

    public void setNames(String string) {
        ++this.curName;
        this.curValue = -1;
        this.names[this.curName] = new String(string);
    }

    public void setXlabel(int n) {
        ++this.curXlabel;
        this.xlabels[this.curXlabel] = n;
    }

    public void setYlabel(int n) {
        ++this.curYlabel;
        this.ylabels[this.curYlabel] = n;
    }

    public void setValues(double d) {
        ++this.curValue;
        this.values[this.curName][this.curValue] = d;
    }

    public void setLow(double d) {
        this.low = d;
    }

    public void goLow() {
        if (this.mod == 1.0) {
            this.mod = this.low;
            return;
        }
        this.mod = 1.0;
    }

    public void update() {
        this.repaint();
    }

    public void paint(Graphics graphics) {
        graphics.setColor(Color.white);
        graphics.fillRect(40, 10, 500, 300);
        graphics.setColor(Color.black);
        graphics.drawRect(40, 10, 500, 300);
        int n = 80;
        graphics.drawLine(80, 280, 500, 280);
        int n2 = 0;
        while (n2 < this.valuesNum) {
            if (n2 % 2 == 0) {
                graphics.setColor(Color.black);
                graphics.drawLine(n, 280, n, 285);
                graphics.drawString("" + this.xlabels[n2], n + 5, 300);
                graphics.setColor(Color.pink);
                graphics.drawLine(n, 280, n, 10);
                graphics.setColor(Color.black);
            }
            n += 420 / this.valuesNum;
            ++n2;
        }
        graphics.drawString("RPM", 270, 330);
        int n3 = 280;
        graphics.drawLine(80, 10, 80, 280);
        int n4 = 0;
        while (n4 < this.curYlabel) {
            graphics.setColor(Color.black);
            graphics.drawLine(80, n3, 75, n3);
            graphics.drawString("" + this.ylabels[n4], 55, n3);
            graphics.setColor(Color.pink);
            graphics.drawLine(80, n3, 500, n3);
            graphics.setColor(Color.black);
            n3 -= 270 / this.curYlabel;
            ++n4;
        }
        graphics.drawString(this.yName, 2, 155);
        graphics.drawLine(80, 280, 500, 280);
        this.decode = 3.3333;
        int n5 = 0;
        while (n5 < this.namesNum) {
            switch (n5) {
                case 0: {
                    graphics.setColor(Color.red);
                    break;
                }
                case 1: {
                    graphics.setColor(Color.blue);
                    break;
                }
                case 2: {
                    graphics.setColor(Color.green);
                    break;
                }
                case 3: {
                    graphics.setColor(Color.black);
                    break;
                }
                case 4: {
                    graphics.setColor(Color.yellow);
                    break;
                }
                default: {
                    graphics.setColor(Color.orange);
                }
            }
            graphics.drawString(this.names[n5], 20 + (n5 + 1) * 60, 380);
            n = 80;
            int n6 = 0;
            while (n6 < this.valuesNum - 1) {
                graphics.drawLine(n, 280 - (int)(this.values[n5][n6] * this.decode / this.mod), n + 420 / this.valuesNum, 280 - (int)(this.values[n5][n6 + 1] * this.decode / this.mod));
                n += 420 / this.valuesNum;
                ++n6;
            }
            ++n5;
        }
    }
}
