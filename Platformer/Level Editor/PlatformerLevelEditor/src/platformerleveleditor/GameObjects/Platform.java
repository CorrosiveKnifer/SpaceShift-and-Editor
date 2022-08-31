/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package platformerleveleditor.GameObjects;

import java.awt.Color;
import java.awt.Graphics;
import java.util.ArrayList;
import platformerleveleditor.Drawable;

/**
 *
 * @author Michael Jordan (14868336)
 */
public class Platform extends Drawable{
    
    public ArrayList<ButtonObject> buttons;
    protected ArrayList<Bound> subBounds;
    protected Bound HeldBound;
    protected int boundIndex;
    
    public Platform(int _x, int _y, int _w, int _h){
        if(_w < 0)
        {
            x = _x + _w;
            w = _w * -1;
        }
        else
        {
            x = _x;
            w = _w;
        }
        
        if(_h < 0)
        {
            y = _y + _h;
            h = _h * -1;
        }
        else
        {
            y = _y;
            h = _h;
        }

        subBounds = new ArrayList<Bound>();
        buttons = new ArrayList<ButtonObject>();
    }
    
    public void AddButton(ButtonObject b)
    {
        buttons.add(b);
        b.y = y - (b.h * 8/10);
    }
    
    @Override
    public void Draw(Graphics g){
        
        g.setColor(Color.white);
        g.fillRect(x, y, w, h);
        if(isShowingBounds)
        {
            UpdateLoc();
            for(int i = 0; i < subBounds.size(); i++)
            {
                g.setColor(Color.red);
                    
                subBounds.get(i).Draw(g);
            }
        }
        for(int i = 0; i < buttons.size(); i++)
        {
                buttons.get(i).Draw(g);
        }
    }
    
    protected void UpdateLoc()
    {
        subBounds.get(0).Update(x - 5, y - 5, 10, 10);
        subBounds.get(1).Update(x + w/2 - 5, y - 5, 10, 10);
        subBounds.get(2).Update(x + w - 5, y - 5, 10, 10);
            
        subBounds.get(3).Update(x - 5, y + h/2 - 5, 10, 10);
            
        subBounds.get(4).Update(x + w - 5, y + h/2 - 5, 10, 10);
            
        subBounds.get(5).Update(x - 5, y + h - 5, 10, 10);
        subBounds.get(6).Update(x + w/2 - 5, y + h - 5, 10, 10);
        subBounds.get(7).Update(x + w - 5, y + h - 5, 10, 10);
    }
    
    @Override
    public String toString()
    {
        return x+"x"+y+"y"+w+"w"+h+"h";
    }
    
    @Override
    public boolean IsOverlapping(int _x, int _y)
    {
        if(IsOverlappingBound(_x, _y))
            return true;
        return super.IsOverlapping(_x, _y);
    }
     
    @Override
    public boolean IsOverlappingBound(int _x, int _y)
    {
        for(int i = 0; i < subBounds.size(); i++)
        {
            if(subBounds.get(i).IsOverlapping(_x, _y))
            {
                if(HeldBound != null)
                {
                    HeldBound.isHeld = false;
                }
                HeldBound = subBounds.get(i);
                HeldBound.isHeld = true;
                boundIndex = i;
                return true;
            }
        }
        if(HeldBound != null)
        {
            HeldBound.isHeld = false;
        }
        HeldBound = null;
        return false;
    }
    
    @Override
    public void SetBoundDisplay(boolean _newBool)
    {
        if(_newBool && isShowingBounds != _newBool)
        {
            subBounds.add(new Bound(x - 5, y - 5, 10, 10));
            subBounds.add(new Bound(x + w/2 - 5, y - 5, 10, 10));
            subBounds.add(new Bound(x + w - 5, y - 5, 10, 10));
            
            subBounds.add(new Bound(x - 5, y + h/2 - 5, 10, 10));
            
            subBounds.add(new Bound(x + w - 5, y + h/2 - 5, 10, 10));
            
            subBounds.add(new Bound(x - 5, y + h - 5, 10, 10));
            subBounds.add(new Bound(x + w/2 - 5, y + h - 5, 10, 10));
            subBounds.add(new Bound(x + w - 5, y + h - 5, 10, 10));
            isShowingBounds = _newBool;
        }
        else if (!_newBool && _newBool != isShowingBounds)
        {
            subBounds.clear();
            isShowingBounds = _newBool;
        }
        
    }
    
    public void UpdateBound(int _dx, int _dy)
    {
        if(HeldBound != null)
        {
            HeldBound.x += _dx;
            HeldBound.y += _dy;
            switch(boundIndex)
            {
                case 0: //Top Left
                    x += _dx;
                    y += _dy;
                    w -= _dx;
                    h -= _dy;
                    break;
                case 1: //Top
                    y += _dy;
                    h -= _dy;
                    break;
                case 2: //Top Right
                    w += _dx;
                    y += _dy;
                    h -= _dy;
                    break;
                case 3: //Left
                    x += _dx;
                    w -= _dx;
                    break;
                case 4: //Right
                    w += _dx;
                    break;
                case 5: //Bot Left
                    x += _dx;
                    w -= _dx;
                    h += _dy;
                    break;
                case 6: //Bot
                    h += _dy;
                    break;
                case 7: //Bot Right
                    w += _dx;
                    h += _dy;
                    break;
            }
        }
    }
    
    public void DropBound()
    {
        if(HeldBound != null)
        {
            HeldBound.isHeld = false;
        }
        
        if(w < 0)
        {
            x = x + w;
            w = w * -1;
        }       
        if(h < 0)
        {
            y = y + h;
            h = h * -1;
        }
        
        HeldBound = null;
        subBounds.get(0).Update(x - 5, y - 5, 10, 10);
        subBounds.get(1).Update(x + w/2 - 5, y - 5, 10, 10);
        subBounds.get(2).Update(x + w - 5, y - 5, 10, 10);
            
        subBounds.get(3).Update(x - 5, y + h/2 - 5, 10, 10);
            
        subBounds.get(4).Update(x + w - 5, y + h/2 - 5, 10, 10);
            
        subBounds.get(5).Update(x - 5, y + h - 5, 10, 10);
        subBounds.get(6).Update(x + w/2 - 5, y + h - 5, 10, 10);
        subBounds.get(7).Update(x + w - 5, y + h - 5, 10, 10);
    }

    @Override
    public void MoveHeld(int _dx, int _dy) {
        if(HeldBound == null)
        {
            this.x += _dx;
            this.y += _dy;
        }
        else
        {
            UpdateBound(_dx, _dy);
        }
    }
    
    public int GetButtonCount()
    {
        return buttons.size();
    }
    
    public void RemoveButton(ButtonObject btn)
    {
        if(buttons.contains(btn))
        {
            buttons.remove(btn);
        }
    }
    
    public void RemoveButtonAt(int index)
    {
        buttons.remove(index);
    }
    
    public ButtonObject GetButtonAt(int index)
    {
        return buttons.get(index);
    }
}
