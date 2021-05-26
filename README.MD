<h1 class="code-line" data-line-start=0 data-line-end=1 ><a id="Base_0"></a>Base</h1>
<h2 class="code-line" data-line-start=1 data-line-end=2 ><a id="_A_Game_Engine__1"></a><em>A “Game Engine”</em></h2>
<p class="has-line-data" data-line-start="3" data-line-end="6">Base is a “Game Engine”, that, in reallity<br>
is just some kind of framework, and a not so good one,<br>
but hey, there’s a way to make a game in this thing (i think).</p>
<p class="has-line-data" data-line-start="7" data-line-end="8">Work in progress is</p>
<ul>
<li class="has-line-data" data-line-start="8" data-line-end="10">the Audio still with tons of bugs</li>
</ul>
<p class="has-line-data" data-line-start="10" data-line-end="12">The two things i do using multithread i’m probably doing wrong<br>
witch is, loading images, and loading and create audio and audio buffer or whatever</p>
<h2 class="code-line" data-line-start=13 data-line-end=14 ><a id="Features_13"></a>Features</h2>
<ul>
<li class="has-line-data" data-line-start="15" data-line-end="16">Draw a quad with a image;</li>
<li class="has-line-data" data-line-start="16" data-line-end="17">Put some static light sources</li>
<li class="has-line-data" data-line-start="17" data-line-end="18">Use ImGui</li>
<li class="has-line-data" data-line-start="18" data-line-end="19">Camera system</li>
<li class="has-line-data" data-line-start="19" data-line-end="21">Particles (veery basic)</li>
</ul>
<h2 class="code-line" data-line-start=21 data-line-end=22 ><a id="Tech_21"></a>Tech</h2>
<p class="has-line-data" data-line-start="23" data-line-end="24">Base use:</p>
<ul>
<li class="has-line-data" data-line-start="25" data-line-end="26"><a href="https://github.com/glfw/glfw">glfw3</a> - window and input events</li>
<li class="has-line-data" data-line-start="26" data-line-end="27"><a href="http://glew.sourceforge.net/">glew</a> - Bind OpenGL functions</li>
<li class="has-line-data" data-line-start="27" data-line-end="28"><a href="https://github.com/g-truc/glm">GLM</a> - for vetors and matrix and math</li>
<li class="has-line-data" data-line-start="28" data-line-end="29">stb_image - load images</li>
<li class="has-line-data" data-line-start="29" data-line-end="30">OpenAL - Sound things</li>
<li class="has-line-data" data-line-start="30" data-line-end="31"><a href="https://github.com/libsndfile/libsndfile">libsndfile</a> - Load sound files</li>
<li class="has-line-data" data-line-start="31" data-line-end="32"></li>
</ul>
<h2 class="code-line" data-line-start=32 data-line-end=33 ><a id="ToDo_32"></a>ToDo:</h2>
<p class="has-line-data" data-line-start="33" data-line-end="34">(in Any particular order)</p>
<ul>
<li class="has-line-data" data-line-start="34" data-line-end="35">Improve audio system</li>
<li class="has-line-data" data-line-start="35" data-line-end="36">Add dynamic light sources</li>
<li class="has-line-data" data-line-start="36" data-line-end="37">Material system of some kind.</li>
<li class="has-line-data" data-line-start="37" data-line-end="38">Improve colisions (maybe)</li>
<li class="has-line-data" data-line-start="38" data-line-end="39">Add some Tile Map system</li>
<li class="has-line-data" data-line-start="39" data-line-end="40">Add other geometrys to the 2d render</li>
<li class="has-line-data" data-line-start="40" data-line-end="41">In text render (using the same 2d render)</li>
<li class="has-line-data" data-line-start="41" data-line-end="42">Add some the text shenanigans (like break line and those things)</li>
</ul>