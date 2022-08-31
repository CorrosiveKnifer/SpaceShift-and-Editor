/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package platformerleveleditor.GameObjects;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import platformerleveleditor.Drawable;

/**
 *
 * @author Michael Jordan (14868336)
 */
public class TextObject extends Drawable{
    
    protected Bound subBound;
    
    private String text;
    private int fontSize;
    
    public TextObject(String txt, int size, int _x, int _y)
    {
        text = txt;
        fontSize = size;
        
        x = _x;
        y = _y - size;
        h = size;
        w = size * txt.length() / 2;
    }
    
    @Override
    public void Draw(Graphics g) {
        g.setColor(Color.red);
        g.drawRect(x, y, w, h);
        g.setColor(Color.white);
        g.setFont(new Font("SansSerif", Font.PLAIN , fontSize));
        g.drawString(text, x, y + fontSize);
    }

    @Override
    public boolean IsOverlappingBound(int _x, int _y) {
        return false;
    }

    @Override
    public void MoveHeld(int _dx, int _dy) {
       this.x += _dx;
       this.y += _dy;
    }
    @Override
    public String toString()
    {
        return x+"x"+(y+fontSize)+"y";
    }
    public String GetText()
    {
        return text;
    }
    
    public int GetFontSize()
    {
        return fontSize;
    }
}
