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
public class PlayerSpawn extends Drawable{
    
    protected Bound subBound;
    
    public PlayerSpawn(int _x, int _y){
        x = _x;
        y = _y;
        w = 45;
        h = 45;
        subBound = new Bound(x + w/2 - 5, y + h/2 - 5, 10, 10);
    }
    
    @Override
    public void Draw(Graphics g) {
        g.setColor(new Color(0.0f, 132/255.0f, 63/255.0f));
        g.fillOval(x, y, w, h);
        if(isShowingBounds)
        {
            subBound.Draw(g);
        }
    }
    
    @Override
    public String toString()
    {
        return x+"x"+y+"y";
    }
    
    @Override
    public boolean IsOverlapping(int _x, int _y)
    {
        return super.IsOverlapping(_x, _y);
    }
    
    public void UpdateBound(int _dx, int _dy)
    {
        if(subBound.isHeld)
        {
            subBound.x += _dx;
            subBound.y += _dy;
            x += _dx;
            y += _dy;
        }
    }
    
    public void DropBound()
    {
        subBound.isHeld = false;
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
}
