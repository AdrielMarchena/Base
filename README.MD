<h1 class="code-line" data-line-start=0 data-line-end=1 ><a id="Base_0"></a>Base</h1>
<h2 class="code-line" data-line-start=1 data-line-end=2 ><a id="_A_Game_Engine__1"></a><em>A “Game Engine”</em></h2>
<p class="has-line-data" data-line-start="3" data-line-end="6">Base is Quotes a Quotes Game Engine, that, in reallity<br>
is just some kind of framework, and a not so good one,<br>
but hey, there’s a way to make a game in this thing.</p>
<p class="has-line-data" data-line-start="7" data-line-end="9">I’m going to add a dynamic light source someday<br>
maybe a meterial system of some kind.</p>
<p class="has-line-data" data-line-start="10" data-line-end="11">Work in progress is</p>
<ul>
<li class="has-line-data" data-line-start="11" data-line-end="13">the Audio still with tons of bugs</li>
</ul>
<p class="has-line-data" data-line-start="13" data-line-end="15">The two things i do using multithread i’m probably doing wrong<br>
witch is, loading images, and loading and create audio and audio buffer or whatever</p>
<h2 class="code-line" data-line-start=16 data-line-end=17 ><a id="Features_16"></a>Features</h2>
<ul>
<li class="has-line-data" data-line-start="18" data-line-end="19">Draw a quad with a image;</li>
<li class="has-line-data" data-line-start="19" data-line-end="20">Put some static light sources</li>
<li class="has-line-data" data-line-start="20" data-line-end="21">Use ImGui</li>
<li class="has-line-data" data-line-start="21" data-line-end="22">Camera system</li>
<li class="has-line-data" data-line-start="22" data-line-end="24">Particles (veery basic)</li>
</ul>
<h2 class="code-line" data-line-start=24 data-line-end=25 ><a id="Tech_24"></a>Tech</h2>
<p class="has-line-data" data-line-start="26" data-line-end="27">Base use:</p>
<ul>
<li class="has-line-data" data-line-start="28" data-line-end="29"><a href="https://github.com/glfw/glfw">glfw3</a> - window and input events</li>
<li class="has-line-data" data-line-start="29" data-line-end="30"><a href="http://glew.sourceforge.net/">glew</a> - Bind OpenGL functions</li>
<li class="has-line-data" data-line-start="30" data-line-end="31"><a href="https://github.com/g-truc/glm">GLM</a> - for vetors and matrix and math</li>
<li class="has-line-data" data-line-start="31" data-line-end="32">stb_image - load images</li>
<li class="has-line-data" data-line-start="32" data-line-end="33">OpenAL - Sound things</li>
<li class="has-line-data" data-line-start="33" data-line-end="34"><a href="https://github.com/libsndfile/libsndfile">libsndfile</a> - Load sound files</li>
</ul>
