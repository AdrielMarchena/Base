#pragma once

namespace Color
{
	constexpr float RBG_NORM = 1.0f / 256.0f;
	//#3380cc
	constexpr glm::vec4 Base_Color{ 0.2f,0.5f,0.8f,1.0f };

	//#cc8033
	constexpr glm::vec4 Base_Complementary{ RBG_NORM * 204.0f, RBG_NORM * 128.0f,RBG_NORM * 51.0f, 1.0f };

	//#5182b3
	constexpr glm::vec4 Base_Monochromatic{ RBG_NORM * 81.0f, RBG_NORM * 130.0f,RBG_NORM * 179.0f, 1.0f };

	//#3333cc
	constexpr glm::vec4 Base_Analogous{ RBG_NORM * 51.0f, RBG_NORM * 51.0f,RBG_NORM * 204.0f, 1.0f };
	//#33cccc
	constexpr glm::vec4 Base_Analogous_2{ RBG_NORM * 51.0f, RBG_NORM * 204.0f,RBG_NORM * 204.0f, 1.0f };

	//#cccc33
	constexpr glm::vec4 Base_SplitComplementary{ RBG_NORM * 204.0f, RBG_NORM * 204.0f,RBG_NORM * 51.0f, 1.0f };
	//#cc3333
	constexpr glm::vec4 Base_SplitComplementary_2{ RBG_NORM * 204.0f, RBG_NORM * 51.0f,RBG_NORM * 51.0f, 1.0f };

	//#80cc33
	constexpr glm::vec4 Base_Triadic{ RBG_NORM * 128.0f, RBG_NORM * 204.0f,RBG_NORM * 51.0f, 1.0f };
	//#cc3380
	constexpr glm::vec4 Base_Triadic_2{ RBG_NORM * 204.0f, RBG_NORM * 51.0f,RBG_NORM * 128.0f, 1.0f };

	//#cc8033
	constexpr glm::vec4 Base_Tetradic{ RBG_NORM * 204.0f, RBG_NORM * 128.0f,RBG_NORM * 51.0f, 1.0f };
	//#80cc33
	constexpr glm::vec4 Base_Tetradic_2{ RBG_NORM * 128.0f, RBG_NORM * 204.0f,RBG_NORM * 51.0f, 1.0f };
	//#8033cc
	constexpr glm::vec4 Base_Tetradic_3{ RBG_NORM * 128.0f, RBG_NORM * 51.0f,RBG_NORM * 204.0f, 1.0f };

	constexpr glm::vec4 White{ 1.0f,1.0f,1.0f,1.0f };
	constexpr glm::vec4 Red{ 1.0f,0.0f,0.0f,1.0f };
	constexpr glm::vec4 Green{ 0.0f,1.0f,0.0f,1.0f };
	constexpr glm::vec4 Blue{ 0.0f,0.0f,1.0f,1.0f };
	constexpr glm::vec4 Black{ 0.0f,0.0f,0.0f,1.0f };
};