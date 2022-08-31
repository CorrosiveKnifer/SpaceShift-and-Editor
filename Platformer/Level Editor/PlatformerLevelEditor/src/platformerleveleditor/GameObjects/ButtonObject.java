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
public class ButtonObject extends Drawable{

    private ArrayList<Door> linked;
    public Platform linkedTo;
    
    public ButtonObject(int _x, int _y)
    {
        x = _x;
        y = _y;
        w = 40;
        h = 20;
        linked = new ArrayList<Door>();
    }
    
    @Override
    public void Draw(Graphics g) {
        g.setColor(Color.red);
        g.fillRect(x , y, w, h);
        
        for(int i = 0; i < linked.size(); i++)
        {
            g.drawLine(x + w/2, y + h/2, linked.get(i).x + linked.get(i).w/2, linked.get(i).y + linked.get(i).h/2);
        }
    }

    @Override
    public boolean IsOverlappingBound(int _x, int _y) {
        return false;
    }
    
    @Override
    public String toString()
    {
        return linkedTo.id+"p"+x+"x"+y+"y";
    }
    
    @Override
    public void MoveHeld(int _dx, int _dy) {
        //this.x += _dx;
        //this.y += _dy;
    }
    
    public void LinkTo(Door d)
    {
        linked.add(d);
    }
    
    public int GetLinkSize()
    {
        return linked.size();
    }
    
    public int GetIDLinkAt(int index)
    {
        return linked.get(index).id;
    }
    
    public void RemoveLink(Door door)
    {
        if(linked.contains(door))
        {
            linked.remove(door);
        }
    }
}
