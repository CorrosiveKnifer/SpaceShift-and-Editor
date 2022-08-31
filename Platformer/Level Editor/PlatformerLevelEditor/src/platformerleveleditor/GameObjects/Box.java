/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package platformerleveleditor.GameObjects;

import java.awt.Color;
import java.awt.Graphics;
import platformerleveleditor.Drawable;

/**
 *
 * @author Michael Jordan (14868336)
 */
public class Box extends Drawable{
    
    public Box(int _x, int _y)
    {
        this.x = _x;
        this.y = _y;
        this.w = 25;
        this.h = 25;
    }
    
    @Override
    public boolean IsOverlappingBound(int _x, int _y) {
        return super.IsOverlapping(_x, _y);
    }

    @Override
    public void MoveHeld(int _dx, int _dy) {
        this.x += _dx;
        this.y += _dy;
    }
    
    @Override
    public String toString()
    {
        return x+"x"+y+"y";
    }
    
    @Override
    public void Draw(Graphics g) 
    {
        g.setColor(new Color(0.52f, 0.39f, 0.26f));
        g.fillRect(x, y, w, h);
        g.setColor(new Color(0.39f, 0.26f, 0.08f));
        g.drawRect(x, y, w, h);
        g.drawRect(x+1, y+1, w-2, h-2);
    }
    
}
