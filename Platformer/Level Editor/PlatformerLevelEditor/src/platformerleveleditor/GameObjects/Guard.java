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
public class Guard extends Drawable{
    protected Bound subBound;

    public Guard(int _x, int _y){
        x = _x;
        y = _y;
        w = 45;
        h = 45;
        subBound = new Bound(x + w/2 - 5, y + h/2 - 5, 10, 10);
    }
    
    @Override
    public void Draw(Graphics g) {
        g.setColor(new Color(178.0f/255.0f,34.0f/255.0f,34.0f/255.0f));
        g.fillOval(x, y, w, h);
        if(isShowingBounds)
        {
            subBound.Draw(g);
        }
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
        return x+"x"+y+"y";
    }
}
