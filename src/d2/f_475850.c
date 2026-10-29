/* 0x475850, places the view's first window (the form's main window) with the placement flags, screen-scaled */

void d2_PlaceMainWindow(int view)
{
    d2_PlaceWindowScaled(d2_Views[view].wins[0]);
}
