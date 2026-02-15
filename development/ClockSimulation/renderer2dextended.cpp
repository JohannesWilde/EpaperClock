#include "renderer2dextended.hpp"


void Renderer2dButton::render(Coordinates2d::Position const & offset,
            Coordinates2d::Dimension const & dimension,
            Color * const data) const
{
    // // First check outer box.
    // Renderer2d::ValidityAndColor renderResult = rendererOutside_.evaluate(position);

    // // If not inside the outer box, then it also won't be in the inner. As this is a button and 1 instance
    // // should not make up the whole display in the foreground, overall it should be more performant to
    // // only evaluate the inner if the outer would have matched as well.
    // if (renderResult.valid)
    // {
    //     Renderer2d::ValidityAndColor const renderResultInside = rendererInside_.evaluate(position);
    //     if (renderResultInside.valid)
    //     {
    //         // If the inner matched as well, overwrite the result.
    //         renderResult = renderResultInside;
    //     }
    //     else
    //     {
    //         // intentionally empty
    //     }
    // }
    // else
    // {
    //     // intentionally empty
    // }

    // return renderResult;
}
