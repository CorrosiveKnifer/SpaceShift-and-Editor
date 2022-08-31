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
public class EndPortal extends Drawable{
    protected Bound subBound;

    public EndPortal(int _x, int _y){
        x = _x;
        y = _y;
        w = 50;
        h = 75;
        subBound = new Bound(x + w/2 - 5, y + h/2 - 5, 10, 10);
    }
    
    @Override
    public void Draw(Graphics g) {
        g.setColor(new Color(124/255.0f, 252/255.0f, 0/255.0f));
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
