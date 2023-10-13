#pragma once
#include "Lu_UIScript.h"
#include "Lu_Texture.h"

namespace Lu
{
    class NumberScript : public UIScript
    {
        enum class eNumber
        {
            Zero = 0,
            One,
            Two,
            Three,
            Four,
            Five,
            Six,
            Seven,
            Eight,
            Nine,
            End
        };

    public:
        NumberScript();
        virtual ~NumberScript();

    private:
        Vector2                     m_arrLT[(int)eNumber::End];
        Vector2                     m_SliceSize;
        int                         m_CurNumber;
        Vector2                     m_TexResolution;

    public:
        void SetNumbersLT(Vector2 _arrLT[], int _Count);
        void SetCurNumber(int _CurNumber);

        void SetSliceSize(Vector2 _SliceSize)
        {
            m_SliceSize = _SliceSize;
        }

        void SetTexResolution(Vector2 _TexResolution)
        {
            m_TexResolution = _TexResolution;
        }

    public:
        virtual void Update() override;

    public:
        void Binds();

    public:
        virtual void SetActive(bool _b) override;
    };
}

