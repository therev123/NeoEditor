/*
 * Copyright 2014 (C) Yannick Pflanzer <neo-engine.de>
 *
 * This file is part of NeoGui.
 *
 * NeoGui is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * NeoGui is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with NeoGui.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Diese Datei ist Teil von NeoGui.
 *
 * NeoGui ist Freie Software: Sie können es unter den Bedingungen
 * der GNU Lesser General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
 * veröffentlichten Version, weiterverbreiten und/oder modifizieren.
 *
 * NeoGui wird in der Hoffnung, dass es nützlich sein wird, aber
 * OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
 * Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
 * Siehe die GNU Lesser General Public License für weitere Details.
 *
 * Sie sollten eine Kopie der GNU Lesser General Public License zusammen mit diesem
 * Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
 */

#include <Canvas.h>
#include <Render.h>

using namespace Neo;

void Canvas::draw()
{
    Render* render = Render::getInstance();
    MRenderingContext* renderingContext = MEngine::getInstance()->getRenderingContext();
    MSystemContext* system = MEngine::getInstance()->getSystemContext();
    system->getScreenSize(&m_width, &m_height);

    unsigned int currentFrameBuffer = 0;
    renderingContext->getCurrentFrameBuffer(&currentFrameBuffer);
    
    if(m_renderToTexture)
    {	    
	    if(m_fbo == 0)
	    {
		    renderingContext->createFrameBuffer(&m_fbo);
	    }
	    
	    renderingContext->bindFrameBuffer(m_fbo);
	    renderingContext->disableDepthTest();
	    renderingContext->attachFrameBufferTexture(M_ATTACH_COLOR0, m_texture->getTextureId());
	    
	    renderingContext->setViewport(0, 0, m_texture->getWidth(), m_texture->getHeight());
	    renderingContext->setClearColor(m_clearColor);
	    renderingContext->clear(M_BUFFER_COLOR);
    }
    
    // Clear the canvas
    render->drawColoredQuad(0,0, m_width, m_height, m_clearColor);

    // Draw all widgets
    for(int i = 0; i < m_widgets.size(); i++)
    {
        m_widgets[i]->draw();
    }
    
    renderingContext->bindFrameBuffer(currentFrameBuffer);
}

void Canvas::update()
{
    // Update all widgets
    for(int i = 0; i < m_widgets.size(); i++)
    {
        m_widgets[i]->update();
    }
}

void Canvas::addWidget(Widget* w)
{
    if(w)
        m_widgets.push_back(w);
}

void Canvas::clear()
{
    m_widgets.clear();
}

void Canvas::enableRenderToTexture(const char* tex)
{
	MLevel* level = MEngine::getInstance()->getLevel();
	MRenderingContext* render = MEngine::getInstance()->getRenderingContext();
	MSystemContext* system = MEngine::getInstance()->getSystemContext();
	
	if(tex)
	{
		// TODO: Proper clean up!
		m_fbo = 0;
		
		char globalFilename[256];
		getGlobalFilename(globalFilename, system->getWorkingDirectory(), tex);
		
		m_texture = level->loadTexture(globalFilename, 0, 0);
	
		m_texture->clear();
		
		unsigned int m_colorTextureId;
		
		render->createTexture(&m_colorTextureId);
		render->bindTexture(m_colorTextureId);
		render->setTextureFilterMode(M_TEX_FILTER_LINEAR, M_TEX_FILTER_LINEAR);
		render->setTextureUWrapMode(M_WRAP_CLAMP);
		render->setTextureVWrapMode(M_WRAP_CLAMP);
		render->texImage(0, m_texture->getWidth(), m_texture->getHeight(), M_UBYTE, M_RGBA, 0);
		
		m_texture->setTextureId(m_colorTextureId);
	
		m_renderToTexture = true;
	}
}
