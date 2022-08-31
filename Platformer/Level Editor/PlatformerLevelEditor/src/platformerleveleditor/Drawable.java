/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package platformerleveleditor;

import java.awt.Graphics;

/**
 *
 * @author Michael Jordan (14868336)
 */
public abstract class Drawable {
    
    public int x, y, w, h;
    public boolean isShowingBounds;
    public int id;
    
    abstract public void Draw(Graphics g);
    abstract public boolean IsOverlappingBound(int _x, int _y); 
    abstract public void MoveHeld(int _dx, int _dy); 
    
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
    
    public void SetBoundDisplay(boolean _newBool)
    {
        isShowingBounds = _newBool;
    }
    
    public void UpdateBound(int _dx, int _dy)
    {
        //Do nothing
    }
    
    public void DropBound()
    {
        //Do Nothing
    }
}
