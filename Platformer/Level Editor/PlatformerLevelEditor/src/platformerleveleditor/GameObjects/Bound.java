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
public class Bound extends Drawable{
    
    public boolean isHeld;
    public Bound(int _x, int _y, int _w, int _h){
        x = _x;
        y = _y;
        w = _w;
        h = _h;
    }
    
    public void Update(int _x, int _y, int _w, int _h)
    {
        x = _x;
        y = _y;
        w = _w;
        h = _h;
    }
    
    public void Draw(Graphics g)
    {
        g.fillRect(x, y, w, h);
    };
    
    public boolean IsOverlapping(int _x, int _y)
    {
        boolean a = _x >= x;
        boolean b = _x <= x + w;
        boolean c = _y >= y;
        boolean d = _y <= y + h;
        
        return a && b && c && d;
    }
    
    @Override
    public String toString()
    {
        return "";
    };

    @Override
    public boolean IsOverlappingBound(int _x, int _y) {
       return IsOverlapping(_x, _y);
    }

    @Override
    public void MoveHeld(int _dx, int _dy) {
        this.x += _dx;
        this.y += _dy;
    }
}
