/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package platformerleveleditor.GameObjects;

import java.awt.Color;
import java.awt.Graphics;

/**
 *
 * @author Michael Jordan (14868336)
 */
public class Door extends Platform{
    
    private Bound scaleBound;
    private boolean IsOpen;
    
    public Door(int _x, int _y, int _w, int _h)
    {
        super(_x, _y, _w, _h);
        IsOpen = false;
        scaleBound = new Bound(x + w /2 - 5, y + _h/2 - 5, 10, 10);
    }
    
    @Override
    public void Draw(Graphics g) 
    {
        g.setColor(new Color(101/255.0f, 67/255.0f, 33/255.0f));
        if(IsOpen)
        {
            g.drawRect(x, y, w, h);
        }
        else
        {
            g.fillRect(x, y, w, h);
        }
        
        if(isShowingBounds)
        {
            for(int i = 0; i < subBounds.size(); i++)
            {
                g.setColor(Color.red);
                    
                subBounds.get(i).Draw(g);
            }
        }
        g.setColor(Color.orange);
        scaleBound.Draw(g);
    }
    
    @Override
    public boolean IsOverlappingBound(int _x, int _y)
    {
        if(scaleBound.IsOverlapping(_x, _y))
        {
            IsOpen = !IsOpen;
        }
        return super.IsOverlappingBound(_x, _y);
    }
    
    @Override
    protected void UpdateLoc()
    {
        super.UpdateLoc();
        scaleBound.Update(x + w /2 - 5, y + h/2 - 5, 10, 10);
    }
    
    @Override
    public void MoveHeld(int _dx, int _dy) {
        if(HeldBound == null)
        {
            this.x += _dx;
            this.y += _dy;
            scaleBound.Update(x + w /2 - 5, y + h/2 - 5, 10, 10);
        }
        else
        {
            UpdateBound(_dx, _dy);
        }
    }
    
    @Override
    public void UpdateBound(int _dx, int _dy)
    {
        super.UpdateBound(_dx, _dy);
        scaleBound.Update(x + w /2 - 5, y + h/2 - 5, 10, 10);
    }
    
    @Override
    public void AddButton(ButtonObject b)
    {
        //Do nothing
    }
    
    public boolean GetOpen()
    {
        return IsOpen;
    }
    
    public void SetOpen(boolean newBool)
    {
        IsOpen = newBool;
    }
}
