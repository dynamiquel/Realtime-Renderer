#pragma once

class Asset
{
public:
    /**
     * \brief AssetManager will call this just before deleting the object.
     */
    virtual void Release() = 0;
};
