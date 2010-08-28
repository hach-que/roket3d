Engine.Unmanaged.Core.RDevice
Engine.Unmanaged.Core


#ifdef DEBUG
#warning This is a test.
#endif

Engine.Unmanaged.Core.RColor AS Engine.Unmanaged.Core.Color
private double p_R;
private double p_G;
private double p_B;


bound RColor()
    {

        this->p_R = 0;
        this->p_G = 0;
        this->p_B = 0;
    }

bound RColor(numeric R, numeric G, numeric B)
    {

        this->p_R = R;
        this->p_G = G;
        this->p_B = B;
    }

bound property numeric R
    {

        get: p_R;
        set: p_R;
    }

bound property numeric G
    {

        get: call GetG;
        set: call SetG;
    }

bound property numeric B
    {

        get: p_B;
        set: p_B;
    }

private numeric GetG()
    {

        return this->p_G;
    }

private SetG(numeric value)
    {

        this->p_G = value;
    }

bound SomeFunc(RDevice dev)
    {

        // We just use the RColor constructor to return one..
        RColor col = new RColor(1, 2, 3);
        return col;
    }

}
