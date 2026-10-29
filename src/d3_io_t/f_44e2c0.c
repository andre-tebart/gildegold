/* 0x44e2c0, frees a stock object that still has references */

void d3io_ReleaseObject(D3IOOBJ *obj)
{
    if (obj != 0 && obj->refs > 0)
        d3iot_DeleteStockObject((D3BGF *)obj);
}
